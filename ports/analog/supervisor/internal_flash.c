
#include "supervisor/internal_flash.h"

#include <stdint.h>
#include <string.h>

#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"

#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"

#include "supervisor/filesystem.h"
#include "supervisor/flash.h"
#include "supervisor/shared/safe_mode.h"

#if CIRCUITPY_USB_DEVICE
#include "supervisor/usb.h"
#endif

// MAX32 HAL Includes
#include "flc.h"
#include "flc_reva.h"
#include "icc.h" // TODO: May have to check that this includes the right ICC e.g. icc_me18.c
#include "mxc_device.h"

/** TODO:
 *      - Define needed Filesystem MACROs in mpconfigport.h
 *      - Define functions to work with the NVM on our MCUs
 *
 *  NOTE:
 *      ANY function which modifies flash contents must execute from a crit section.
 *      This is because FLC functions are loc'd in RAM, and any ISR executing
 *      from Flash will trigger a HardFault.
 *
 *      An alternative would be to initialize with NVIC_SetRAM(),
 *      which makes ISRs execute from RAM.
 *
 * NOTE:
 *      Additionally, any code that modifies flash contents must disable the
 *      cache. Turn off ICC0 any time flash is to be modified. Remember to re-enable if using.
 */

#define NO_CACHE        0xffffffff
#define MAX_CACHE       0x4000

typedef struct {
    uint32_t base_addr;
    uint32_t sector_size;
    uint32_t num_sectors;
} flash_layout_t;

#ifdef MAX32690
static const flash_layout_t flash_layout[] = {
    { 0x1000000, 0x4000, 192},
    { 0x1030000, 0x2000, 32 },
};
static uint8_t _flash_cache[0x4000] __attribute__((aligned(4)));
#endif

// Address of the flash sector currently being cached
static uint32_t _cache_addr_in_flash = NO_CACHE;

static inline uint32_t block2addr(uint32_t block) {
    if (block > 0 && block < INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS) {
        return CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_START_ADDR + block * FILESYSTEM_BLOCK_SIZE;
    }
    else return -1;
}

int flash_get_sector_info(uint32_t addr, uint32_t *start_addr, uint32_t *size) {
    // This function should return -1 in the event of errors.
    if (addr >= flash_layout[0].base_addr) {
        uint32_t sector_index = 0;
        if (MP_ARRAY_SIZE(flash_layout) == 1) {
            sector_index = (addr - flash_layout[0].base_addr) / flash_layout[0].sector_size;
            if (sector_index >= flash_layout[0].num_sectors) {
                return -1;
            }
            if (start_addr) {
                *start_addr = flash_layout[0].base_addr + (sector_index * flash_layout[0].sector_size);
            }
            if (size) {
                *size = flash_layout[0].sector_size;
            }
            return sector_index;
        }

        for (uint8_t i = 0; i < MP_ARRAY_SIZE(flash_layout); ++i) {
            for (uint8_t j = 0; j < flash_layout[i].num_sectors; ++j) {
                uint32_t sector_start_next = flash_layout[i].base_addr
                    + (j + 1) * flash_layout[i].sector_size;
                if (addr < sector_start_next) {
                    if (start_addr) {
                        *start_addr = flash_layout[i].base_addr
                            + j * flash_layout[i].sector_size;
                    }
                    if (size) {
                        *size = flash_layout[i].sector_size;
                    }
                    return sector_index;
                }
                ++sector_index;
            }
        }
    }
    return -1;
}

void supervisor_flash_init(void) {
    // No initialization needed.
    // Pay attention to the note at the top of this file!
}

uint32_t supervisor_flash_get_block_size(void) {
    return FILESYSTEM_BLOCK_SIZE;
}

uint32_t supervisor_flash_get_block_count(void) {
    return INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS;
}

// Flush the flash page that is currently cached
void port_internal_flash_flush(void) {
    // Flash has not been cached
    if (_cache_addr_in_flash == NO_CACHE) {
        return;
    }
    uint32_t sector_start, sector_size = 0xffffffff;

    // Clear & enable flash interrupt flags
    MXC_FLC_EnableInt(MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);

    // Figure out the sector of flash we're targeting
    if (flash_get_sector_info(_cache_addr_in_flash, &sector_start, &sector_size) == -1) {
        // If not in valid sector, just release the cache and return
        supervisor_flash_release_cache();
        return;
    }

    // if invalid sector or sector size > the size of our cache, reset with flash fail
    if (sector_size > sizeof(_flash_cache) || sector_start == 0xffffffff) {
        reset_into_safe_mode(SAFE_MODE_FLASH_WRITE_FAIL);
    }

    // skip if the data in cache is the same as what's already there
    if (memcmp(_flash_cache, (void *)_cache_addr_in_flash, FLASH_PAGE_SIZE) != 0) {
        uint32_t error;

        // buffer for the page of flash
        uint32_t page_buffer[FLASH_PAGE_SIZE >> 2] = {
            0xFFFFFFFF
        }; // bytes per page / 4 bytes = # of uint32_t

        // Unlock Flash
        MXC_FLC0->ctrl = (MXC_FLC0->ctrl & ~MXC_F_FLC_REVA_CTRL_UNLOCK) | MXC_S_FLC_REVA_CTRL_UNLOCK_UNLOCKED;

        /*** ERASE FLASH PAGE ***/
        MXC_CRITICAL(
            // buffer the page
            MXC_FLC_Read(sector_start, page_buffer, sector_size);
            // Erase page & error check
            error = MXC_FLC_PageErase(sector_start);
        );
        if (error != E_NO_ERROR) {
            // lock flash & reset
            MXC_FLC0->ctrl = (MXC_FLC0->ctrl & ~MXC_F_FLC_REVA_CTRL_UNLOCK) | MXC_S_FLC_REVA_CTRL_UNLOCK_LOCKED;
            reset_into_safe_mode(SAFE_MODE_FLASH_WRITE_FAIL);
        }

        /*** RE-WRITE FLASH PAGE w/ CACHE DATA ***/
        MXC_CRITICAL(
            // ret = program the flash page with cache data (for loop)
            for (int i = 0; i < (sector_size >> 2); i++) {
                error = MXC_FLC_Write32(_cache_addr_in_flash + 4 * i, _flash_cache[i]);
            }
        );
        if (error != E_NO_ERROR) {
            // lock flash & reset
            MXC_FLC0->ctrl = (MXC_FLC0->ctrl & ~MXC_F_FLC_REVA_CTRL_UNLOCK) | MXC_S_FLC_REVA_CTRL_UNLOCK_LOCKED;
            reset_into_safe_mode(SAFE_MODE_FLASH_WRITE_FAIL);
        }

        // Lock flash & exit
        MXC_FLC0->ctrl = (MXC_FLC0->ctrl & ~MXC_F_FLC_REVA_CTRL_UNLOCK) | MXC_S_FLC_REVA_CTRL_UNLOCK_LOCKED;
    }

}

mp_uint_t supervisor_flash_read_blocks(uint8_t *dest, uint32_t block, uint32_t num_blocks) {
    // Find the address of the block we want to read
    int src_addr = block2addr(block);
    if (src_addr == -1) {
        // bad block num
        return false;
    }

    uint32_t sector_size, sector_start;
    if (flash_get_sector_info(src_addr, &sector_start, &sector_size) == -1) {
        // bad sector idx
        return false;
    }

    // Find how many blocks left in sector
    uint32_t blocks_in_sector = (sector_size - (src_addr - sector_start)) / FILESYSTEM_BLOCK_SIZE;

    // If the whole read is inside the cache, then read cache
    if (num_blocks <= blocks_in_sector && _cache_addr_in_flash == sector_start) {
        memcpy(dest, (_flash_cache + (src_addr - sector_start)), FILESYSTEM_BLOCK_SIZE * num_blocks);
    } else {
        supervisor_flash_flush();
        /** NOTE:   The MXC_FLC_Read function executes from SRAM and does some more error checking
        *          than memcpy does. Will use it for now.
        */
        MXC_FLC_Read(dest, (uint8_t *)src_addr, FILESYSTEM_BLOCK_SIZE * num_blocks);
    }
    return 0; // success
}

mp_uint_t supervisor_flash_write_blocks(const uint8_t *src, uint32_t block_num, uint32_t num_blocks) {
    uint32_t count=0;
    uint32_t sector_size=0;
    uint32_t sector_start=0;

    while (num_blocks > 0) {
        uint32_t const dest_addr = block2addr(block_num);
        // bad block number passed in
        if (dest_addr == -1) {
            return false;
        }

        // Implementation is from STM port
        // NOTE:    May replace later, but this port had a method
        //          that seemed to make sense across multiple devices.
        if (flash_get_sector_info(dest_addr, &sector_start, &sector_size) == -1) {
            reset_into_safe_mode(SAFE_MODE_FLASH_WRITE_FAIL);
        }

        // fail if sector size is greater than cache size
        if (sector_size > sizeof(_flash_cache)) {
            reset_into_safe_mode(SAFE_MODE_FLASH_WRITE_FAIL);
        }

        // Find the number of blocks left within this sector
        // BLOCK_NUM = (SECTOR SIZE - BLOCK OFFSET within sector)) / BLOCK_SIZE
        count = (sector_size - (dest_addr - sector_start)) / FILESYSTEM_BLOCK_SIZE;
        count = MIN(num_blocks, count);

        // if we're not at the start of a sector, copy the whole sector to cache
        if (_cache_addr_in_flash != sector_start) {
            // Flush cache first
            supervisor_flash_flush();

            _cache_addr_in_flash = sector_start;

            // Copy the whole sector into cache
            memcpy(_flash_cache, (void *)sector_start, sector_size);
        }

        // Overwrite the cache with source data passed in
        memcpy(_flash_cache + (dest_addr - sector_start), src, count * FILESYSTEM_BLOCK_SIZE);

        block_num += count;
        src += count * FILESYSTEM_BLOCK_SIZE;
        num_blocks -= count;
    }
    return 0; // success
}

void supervisor_flash_release_cache(void) {
    // Flush the cache for ARM M4
    MXC_ICC_Flush(MXC_ICC0);
    MXC_ICC_Flush(MXC_ICC1);

    // Clear the line fill buffer by reading 2 pages from flash
    volatile uint32_t *line_addr;
    volatile uint32_t line;
    line_addr = (uint32_t *)(MXC_FLASH_MEM_BASE);
    line = *line_addr;
    line_addr = (uint32_t *)(MXC_FLASH_MEM_BASE + MXC_FLASH_PAGE_SIZE);
    line = *line_addr;
    (void)line; // Silence build warnings that this variable is not used.

    // Invalidate the current cache
    _cache_addr_in_flash = NO_CACHE;
}

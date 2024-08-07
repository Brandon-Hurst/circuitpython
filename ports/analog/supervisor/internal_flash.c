
#include "internal_flash.h"

#include <stdint.h>
#include <string.h>

#include "py/obj.h"
#include "supervisor/flash.h"
#include "supervisor/shared/safe_mode.h"

#ifdef MAX32690
static const flash_layout_t flash_layout[] = {
    { INTERNAL_FLASH_FILESYSTEM_START_ADDR, 0x1000, 256 },
};
static uint8_t _flash_cache[0x1000] __attribute__((aligned(4)));
#endif

#define NO_CACHE        0xffffffff
#define MAX_CACHE       0x4000

static uint32_t _cache_flash_addr = NO_CACHE;



uint32_t flash_get_sector_info(uint32_t addr, uint32_t *start_addr, uint32_t *size) {
    // TODO: Implement
    return 0;   // todo dangerous - shouldn't this raise an exception?
}

void supervisor_flash_init(void) {
    // TODO: Implement
}

uint32_t supervisor_flash_get_block_size(void) {
    return FILESYSTEM_BLOCK_SIZE;
}

uint32_t supervisor_flash_get_block_count(void) {
    return INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS;
}

void port_internal_flash_flush(void) {
    // TODO: Implement
}

static uint32_t convert_block_to_flash_addr(uint32_t block) {
    if (0 <= block && block < INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS) {
        // a block in partition 1
        return INTERNAL_FLASH_FILESYSTEM_START_ADDR + block * FILESYSTEM_BLOCK_SIZE;
    }
    // bad block
    return -1;
}


mp_uint_t supervisor_flash_read_blocks(uint8_t *dest, uint32_t block, uint32_t num_blocks) {
    // TODO: Implement
    return 0; // success
}


mp_uint_t supervisor_flash_write_blocks(const uint8_t *src, uint32_t block_num, uint32_t num_blocks) {
    // TODO: Implement
    return 0; // success
}


void supervisor_flash_release_cache(void) {
    // TODO: Implement
}

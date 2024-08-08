
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "py/mpconfig.h"

/** TODO:   Validate these settings can work
 *  NOTE:   This may necessitate some customization of the linker file.
*/
#ifdef MAX32690
#define MAX32_FLASH_SIZE  0x300000 // 3 MB
#define INTERNAL_FLASH_FILESYSTEM_SIZE 0x100000 // 1 MB
#define INTERNAL_FLASH_FILESYSTEM_START_ADDR 0x10200000 // Load into the last MB of code/data storage???
#endif

#define INTERNAL_FLASH_FILESYSTEM_NUM_BLOCKS (INTERNAL_FLASH_FILESYSTEM_SIZE / FILESYSTEM_BLOCK_SIZE)

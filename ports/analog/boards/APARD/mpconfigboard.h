// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// Use the MP_WEAK supervisor/shared/board.c versions of routines not defined here.
// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2016 Glenn Ruben Bakke
// SPDX-FileCopyrightText: Copyright (c) 2018 Dan Halbert for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

#define MICROPY_HW_BOARD_NAME       "APARD32690"
#define MICROPY_HW_MCU_NAME         "max32690"

#define FLASH_SIZE                  (0x300000) // 3MiB
#define FLASH_PAGE_SIZE             (0x4000)   // 16384 byte pages (16 KiB)

#define BOARD_HAS_CRYSTAL 1

// todo: figure out a way to smartly set this up based on storage considerations
#if INTERNAL_FLASH_FILESYSTEM
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_START_ADDR (0x1020000UL) // for MAX32690
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_SIZE (512 * 1024)
#else
#define CIRCUITPY_INTERNAL_FLASH_FILESYSTEM_SIZE (0)
#endif

# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

INTERNAL_FLASH_FILESYSTEM = 1
# FLASH: 	0x10000000 to 0x10340000
# SRAM: 	0x20000000 to 0x20100000

USB_PRODUCT = "MAX32690 APARD"
USB_MANUFACTURER = "Analog Devices, Inc."
# CFLAGS+=-DEXT_FLASH_MX25

MCU_SERIES=max32
MCU_VARIANT=max32690

CFLAGS += -DHAS_TRNG=1

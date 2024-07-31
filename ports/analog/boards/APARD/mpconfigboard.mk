# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

MCU_SERIES=max32
MCU_VARIANT=max32690

CFLAGS+=-DEXT_FLASH_MX25

MISC_DRIVERS_DIR ?= $(MAXIM_PATH)/Libraries/MiscDrivers
BOARD_DIR = $(LIBS_DIR)/Boards/$(MCU_VARIANT_UPPER)/$(BOARD)

# Where to find BSP source files
VPATH += $(BOARD_DIR)/Source
VPATH += $(MISC_DRIVERS_DIR)
VPATH += $(MISC_DRIVERS_DIR)/LED
VPATH += $(MISC_DRIVERS_DIR)/PushButton
VPATH += $(MISC_DRIVERS_DIR)/ExtMemory

# Where to find BSP header files
INC += $(BOARD_DIR)/Include
INC += $(MISC_DRIVERS_DIR)
INC += $(MISC_DRIVERS_DIR)/LED
INC += $(MISC_DRIVERS_DIR)/PushButton
INC += $(MISC_DRIVERS_DIR)/ExtMemory

# Source files for this board (add path to VPATH below)
SRC_C += board.c
SRC_C += led.c
SRC_C += pb.c
SRC_C += mx25.c
SRC_C += stdio.c

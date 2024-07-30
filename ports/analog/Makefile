# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

include ../../py/circuitpy_mkenv.mk

CROSS_COMPILE = arm-none-eabi-
USB_NUM_ENDPOINT_PAIRS = 0

MCU_SERIES_LOWER = $(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')
MCU_SERIES_UPPER = $(shell echo $(MCU_SERIES) | tr '[:lower:]' '[:upper:]')
MCU_VARIANT_LOWER = $(shell echo $(MCU_VARIANT) | tr '[:upper:]' '[:lower:]')
MCU_VARIANT_UPPER = $(shell echo $(MCU_VARIANT) | tr '[:lower:]' '[:upper:]')

INC += -I.
INC += -I../.. # core cktpyton directory
INC += -I$(BUILD) # build for libs
INC += -I$(BUILD)/genhdr

# *******************************************************************************
# CMSIS files
INC += -I./msdk/Libraries/CMSIS/5.9.0/Core/Include
INC += -I./msdk/Libraries/CMSIS/Device/Maxim/$(MCU_UPPER)/Include
# *******************************************************************************
# MSDK-Specific Definitions (tbd...)
#
#
MAXIM_PATH = ./msdk
LIBS_DIR := $(MAXIM_PATH)/Libraries
CMSIS_ROOT := $(LIBS_DIR)/CMSIS

TARGET = $(MCU_VARIANT_UPPER)
TARGET_LC = $(MCU_VARIANT_LOWER)
TARGET_UC = $(MCU_VARIANT_UPPER)

COMPILER := GCC
LD_FILE := $(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/$(MCU_VARIANT_LOWER).ld

# Include MSDK Libs. Only builds MSDK-BSP & PeriphDrivers by default
include $(LIBS_DIR)/libs.mk

# *******************************************************************************
# Board files (bsp is in libs for now)
INC += -I./boards
INC += -I./boards/$(BOARD)
# *******************************************************************************
# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_SUPERVISOR)
# Sources that only hold QSTRs after pre-processing.
SRC_QSTR_PREPROCESSOR +=

LIBS := -lgcc -lc

# OBJ includes
OBJ += $(PY_O) $(SUPERVISOR_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

# Default build target
all: $(BUILD)/firmware.elf

$(BUILD)/firmware.elf: $(OBJ)
	$(STEPECHO) "LINK $@"
	$(Q)echo $^ > $(BUILD)/firmware.objs
	$(Q)$(CC) -o $@ $(LDFLAGS) @$(BUILD)/firmware.objs -Wl, --print-memory-usage -Wl,--start-group $(LIBS) -Wl,--end-group
	$(Q)$(SIZE) $@ | $(PYTHON) $(TOP)/tools/build_memory_info.py $(LD_FILE) $(BUILD)

include $(TOP)/py/mkrules.mk

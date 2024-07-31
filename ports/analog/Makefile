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

TARGET_UC = MCU_VARIANT_UPPER
TARGET_LC = MCU_VARIANT_LOWER

INC += -I.
INC += -I../.. # core cktpyton directory
INC += -I$(BUILD) # build for libs
INC += -I$(BUILD)/genhdr
INC += -I../../supervisor
INC += -I./../../lib/cmsis/inc
INC += -I./boards/$(BOARD)
INC += -I../../py

# FIXME: Start with DEBUG ONLY settings for now
CFLAGS += -ggdb3
OPTIMIZATION_FLAGS = -Og

CFLAGS += $(INC) -Wall -Werror -std=gnu11 -nostdlib -fshort-enums $(BASE_CFLAGS) $(CFLAGS_MOD) $(COPT) -Werror=missing-prototypes
CFLAGS += -D__START=main

# *******************************************************************************
# CMSIS files
INC += -I./msdk/Libraries/CMSIS/5.9.0/Core/Include
INC += -I./msdk/Libraries/CMSIS/Device/Maxim/$(MCU_VARIANT_UPPER)/Include

# Include periphdriver headers
INC += -I./msdk/Libraries/PeriphDrivers/Include/$(MCU_VARIANT_UPPER)
SRC_C +=

# *******************************************************************************
# MSDK-Specific Definitions (tbd...)
#
#
MAXIM_PATH = ./msdk
LIBS_DIR = $(MAXIM_PATH)/Libraries
CMSIS_ROOT = $(LIBS_DIR)/CMSIS

export TARGET_LC
export TARGET_UC
export CMSIS_ROOT

TARGET = $(MCU_VARIANT_UPPER)
TARGET_LC = $(MCU_VARIANT_LOWER)
TARGET_UC = $(MCU_VARIANT_UPPER)

COMPILER := GCC
LD_FILE := $(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/$(MCU_VARIANT_LOWER).ld
LDFLAGS += -mthumb -mcpu=cortex-m4

CPPFLAGS += -D TARGET_REV=0x4131

# Include MSDK Libs. Only builds MSDK-BSP & PeriphDrivers by default
# TODO: Create an equivalent include for CKTPY build system
# include $(LIBS_DIR)/libs.mk
include ./mcu/$(MCU_VARIANT_LOWER)/$(MCU_VARIANT_LOWER)_files.mk

# Include CMSIS make script for the target MCU
# TODO: Create an equivalent include for CKTPY build system
# FIXME: This file includes gcc.mk from CMSIS, which modifies the target rules. Whoops!
# include $(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/$(COMPILER)/$(MCU_VARIANT_LOWER).mk

# TODO: Add extra necessary sources as necessary here

# *******************************************************************************
# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_SUPERVISOR) $(SRC_CIRCUITPY_COMMON)
# Sources that only hold QSTRs after pre-processing.
SRC_QSTR_PREPROCESSOR +=

LIBS := -lgcc -lc

# SRC_COMMON_HAL_EXPANDED = $(addprefix shared-bindings/, $(SRC_COMMON_HAL)) \
#                           $(addprefix shared-bindings/, $(SRC_BINDINGS_ENUMS)) \
#                           $(addprefix common-hal/, $(SRC_COMMON_HAL))

# SRC_SHARED_MODULE_EXPANDED = $(addprefix shared-bindings/, $(SRC_SHARED_MODULE)) \
#                              $(addprefix shared-module/, $(SRC_SHARED_MODULE)) \
#                              $(addprefix shared-module/, $(SRC_SHARED_MODULE_INTERNAL))

# SRC_COMMON_HAL_SHARED_MODULE_EXPANDED = $(sort $(SRC_COMMON_HAL_EXPANDED) $(SRC_SHARED_MODULE_EXPANDED))


# OBJ includes
OBJ += $(PY_O) $(SUPERVISOR_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
# OBJ += $(addprefix $(BUILD)/, $(SRC_COMMON_HAL_SHARED_MODULE_EXPANDED:.c=.o))
ifeq ($(INTERNAL_LIBM),1)
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))
endif
OBJ += $(addprefix $(BUILD)/, $(SRC_CIRCUITPY_COMMON:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_MOD:.c=.o))

# Default build target
all: $(BUILD)/firmware.elf

$(BUILD)/firmware.elf: $(OBJ)
	$(STEPECHO) "LINK $@"
	$(Q)echo $^ > $(BUILD)/firmware.objs
	$(Q)$(CC) -o $@ $(LDFLAGS) @$(BUILD)/firmware.objs -Wl, --print-memory-usage -Wl,--start-group $(LIBS) -Wl,--end-group
	$(Q)$(SIZE) $@ | $(PYTHON) $(TOP)/tools/build_memory_info.py $(LD_FILE) $(BUILD)


include $(TOP)/py/mkrules.mk

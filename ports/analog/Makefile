# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
#
# SPDX-License-Identifier: MIT

# Includes mpconfigboard.mk & mpconfigport.mk,
# along with numerous other shared environment makefiles.
include ../../py/circuitpy_mkenv.mk

CROSS_COMPILE = arm-none-eabi-

# MCU_SERIES e.g. 	"max32"
# MCU_VARIANT e.g. 	"max32690"
# defined in mpconfigboard.mk
MCU_SERIES_LOWER := $(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')
MCU_SERIES_UPPER := $(shell echo $(MCU_SERIES) | tr '[:lower:]' '[:upper:]')
MCU_VARIANT_LOWER := $(shell echo $(MCU_VARIANT) | tr '[:upper:]' '[:lower:]')
MCU_VARIANT_UPPER := $(shell echo $(MCU_VARIANT) | tr '[:lower:]' '[:upper:]')

# *******************************************************************************
#### MSDK INCLUDES ####
# Necessary for msdk makefiles
TARGET := $(MCU_VARIANT_UPPER)
TARGET_UC := $(MCU_VARIANT_UPPER)
TARGET_LC := $(MCU_VARIANT_LOWER)

MSDK_ROOT = ./msdk
MSDK_LIBS = $(MSDK_ROOT)/Libraries
CMSIS_ROOT = $(MSDK_LIBS)/CMSIS
ADI_PERIPH = $(MSDK_ROOT)/Libraries/PeriphDrivers
ADI_MISC_DRIVERS_DIR ?= $(MSDK_LIBS)/MiscDrivers
ADI_BOARD_DIR = $(MSDK_LIBS)/Boards/$(MCU_VARIANT_UPPER)/$(BOARD)

# For DEBUG
$(info MSDK_ROOT is $(MSDK_ROOT))
$(info MSDK_LIBS is $(MSDK_LIBS))
$(info CMSIS_ROOT is $(CMSIS_ROOT))
$(info ADI_PERIPH is $(ADI_PERIPH))
$(info ADI_MISC_DRIVERS_DIR is $(ADI_MISC_DRIVERS_DIR))
$(info ADI_BOARD_DIR is $(ADI_BOARD_DIR))

# -----------------
# Sources & Include
# -----------------
# Define max32 die type for PeriphDriver Includes
# default to me18 for max32690
# more info:
# https://analogdevicesinc.github.io/msdk//USERGUIDE/#die-types-to-part-numbers
ifeq ($(MCU_VARIANT_LOWER), "max32690")
DIE_TYPE=me18
else
DIE_TYPE=me18
endif

PERIPH_SRC = $(ADI_PERIPH)/Source

INC += -I.
INC += -I../..
INC += -I$(BUILD)
INC += -I$(BUILD)/genhdr
INC += -I./../../lib/cmsis/inc
INC += -I./boards/
INC += -I./boards/$(BOARD)
INC += -I./peripherals/
INC += -I../../lib/mp-readline

INC += \
	-I$(TOP)/$(BOARD_PATH) \
	-I$(TOP)/lib/cmsis/inc \
	-I$(CMSIS_ROOT)/Include \
	-I$(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Include \
	-I$(ADI_PERIPH)/Include/$(MCU_VARIANT_UPPER) \
	-I$(PERIPH_SRC)/SYS \
	-I$(PERIPH_SRC)/GPIO \
	-I$(PERIPH_SRC)/CTB \
	-I$(PERIPH_SRC)/ICC \
	-I$(PERIPH_SRC)/FLC \
	-I$(PERIPH_SRC)/UART \

INC += -I$(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/GCC

SRC_C += \
	$(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/heap.c \
	$(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/system_$(MCU_VARIANT_LOWER).c \
	$(PERIPH_SRC)/SYS/mxc_assert.c \
	$(PERIPH_SRC)/SYS/mxc_delay.c \
	$(PERIPH_SRC)/SYS/mxc_lock.c \
	$(PERIPH_SRC)/SYS/nvic_table.c \
	$(PERIPH_SRC)/SYS/pins_$(DIE_TYPE).c \
	$(PERIPH_SRC)/SYS/sys_$(DIE_TYPE).c \
	$(PERIPH_SRC)/CTB/ctb_$(DIE_TYPE).c \
	$(PERIPH_SRC)/CTB/ctb_reva.c \
	$(PERIPH_SRC)/CTB/ctb_common.c \
	$(PERIPH_SRC)/FLC/flc_common.c \
	$(PERIPH_SRC)/FLC/flc_$(DIE_TYPE).c \
	$(PERIPH_SRC)/FLC/flc_reva.c \
	$(PERIPH_SRC)/GPIO/gpio_common.c \
	$(PERIPH_SRC)/GPIO/gpio_$(DIE_TYPE).c \
	$(PERIPH_SRC)/GPIO/gpio_reva.c \
	$(PERIPH_SRC)/ICC/icc_$(DIE_TYPE).c \
	$(PERIPH_SRC)/ICC/icc_reva.c \
	$(PERIPH_SRC)/UART/uart_common.c \
	$(PERIPH_SRC)/UART/uart_$(DIE_TYPE).c \
	$(PERIPH_SRC)/UART/uart_revb.c \

# *******************************************************************************
### Compiler & Linker Flags ###
COMPILER ?= GCC

ifeq ($(COMPILER), GCC)
STARTUPFILE = $(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/GCC/startup_$(MCU_VARIANT_LOWER).s
LINKERFILE = $(CMSIS_ROOT)/Device/Maxim/$(MCU_VARIANT_UPPER)/Source/GCC/$(MCU_VARIANT_LOWER).ld
LDFLAGS += -nostartfiles --specs=nosys.specs --specs=nano.specs
endif

SRC_S += $(STARTUPFILE)

# Needed to compile some MAX32 headers
CFLAGS += -D$(MCU_VARIANT_UPPER) \
			-DTARGET_REV=0x4131 \
			-DTARGET=$(MCU_VARIANT_UPPER) \
			-DFLASH_ORIGIN=0x10000000 \
			-DFLASH_SIZE=0x340000 \
			-DSRAM_ORIGIN=0x20000000 \
			-DSRAM_SIZE=0x100000 \
			-DIAR_PRAGMAS=0 \

CFLAGS +=	-Wno-error=unused-parameter \
				-Wno-error=strict-prototypes \
				-Wno-error=old-style-declaration \
				-Wno-error=sign-compare \
				-Wno-error=cast-qual \
				-Wno-lto-type-mismatch \

CPU_CORE=cortex-m4
CFLAGS += -mthumb -mcpu=$(CPU_CORE) -mfloat-abi=hard -mfpu=fpv4-sp-d16

# NOTE: Start with DEBUG ONLY settings for now
ifeq ($(DEBUG),)
DEBUG ?= 1
endif

ifeq ($(DEBUG),1)
CFLAGS += -ggdb3
COPT = -Og
else
COPT += -O2
endif

# TODO: Add TUSB in
# TinyUSB CFLAGS
# CFLAGS += \
# 	DCFG_TUSB_MCU=OPT_MCU_MAX32690 \
# 	-DBOARD_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED

LDFLAGS = $(CFLAGS) -Wl,-nostdlib -Wl,-T,$(LINKERFILE) -Wl,-Map=$@.map -Wl,-cref -Wl,-gc-sections -specs=nano.specs
LIBS := -lgcc -lc -lnosys

# If not using CKTPY mathlib, use toolchain mathlib
ifndef INTERNAL_LIBM
LIBS += -lm
endif


# *******************************************************************************


# TODO: TinyUSB Includes once USB is ready
# INC += -I../../lib/tinyusb/src
# INC += -I../../supervisor/shared/usb

# TODO: Add source files for TinyUSB once the PR goes through
# SRC_C += lib/tinyusb/src/portable/mentor/musb/dcd_musb.c

SRC_C += \
	boards/$(BOARD)/board.c \
	background.c \
	mphalport.c \

CFLAGS += $(INC) -Wall -std=gnu11 -nostartfiles $(BASE_CFLAGS) $(COPT)
# CFLAGS += $(CFLAGS_MOD)

# *******************************************************************************
### PORT-DEFINED BUILD RULES ###
# This section attempts to build the Python core, the supervisor, and any
# port-provided source code.
#
# QSTR sources are provided for the initial build step, which generates
# Python constants to represent C data which gets passed into the GC.

# TODO: Start adding port-defined Common HAL components
# SRC_COMMON_HAL_EXPANDED = $(addprefix shared-bindings/, $(SRC_COMMON_HAL)) \
#                           $(addprefix shared-bindings/, $(SRC_BINDINGS_ENUMS)) \
#                           $(addprefix common-hal/, $(SRC_COMMON_HAL))

# SRC_SHARED_MODULE_EXPANDED = $(addprefix shared-bindings/, $(SRC_SHARED_MODULE)) \
#                              $(addprefix shared-module/, $(SRC_SHARED_MODULE)) \
#                              $(addprefix shared-module/, $(SRC_SHARED_MODULE_INTERNAL))

# OBJ includes
OBJ += $(PY_O) $(SUPERVISOR_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
# OBJ += $(addprefix $(BUILD)/, $(SRC_COMMON_HAL_EXPANDED:.c=.o))
# OBJ += $(addprefix $(BUILD)/, $(SRC_SHARED_MODULE_EXPANDED:.c=.o))
ifeq ($(INTERNAL_LIBM),1)
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))
endif
OBJ += $(addprefix $(BUILD)/, $(SRC_CIRCUITPY_COMMON:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
# OBJ += $(addprefix $(BUILD)/, $(SRC_MOD:.c=.o))

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_SUPERVISOR) $(SRC_CIRCUITPY_COMMON)
# SRC_QSTR += $(SRC_MOD) $(SRC_COMMON_HAL_EXPANDED) $(SRC_SHARED_MODULE_EXPANDED)

# Sources that only hold QSTRs after pre-processing.
SRC_QSTR_PREPROCESSOR +=

# Default build target
all: $(BUILD)/firmware.elf

clean-max32:
	rm -rf build-*

# Optional flash option when running within an installed MSDK to use OpenOCD
# Mainline OpenOCD does not yet have the MAX32's flash algorithm integrated.
# If the MSDK is installed, flash-msdk can be run to utilize the the modified
# openocd with the algorithms
MAXIM_PATH := $(subst \,/,$(MAXIM_PATH))
flash-msdk:
	$(MAXIM_PATH)/Tools/OpenOCD/openocd -s $(MAXIM_PATH)/Tools/OpenOCD/scripts \
		-f interface/cmsis-dap.cfg -f target/max32690.cfg \
		-c "program $(BUILD)/$(PROJECT).elf verify; init; reset; exit"

# flash target using JLink
JLINK_DEVICE = $(MCU_VARIANT_LOWER)
flash: flash-jlink

$(BUILD)/firmware.elf: $(OBJ) $(LINKERFILE)
	$(STEPECHO) "LINK $@"
	$(Q)echo $(OBJ) > $(BUILD)/firmware.objs
	$(Q)$(CC) -o $@ $(LDFLAGS) @$(BUILD)/firmware.objs -Wl,--print-memory-usage -Wl,--start-group $(LIBS) -Wl,--end-group
	$(Q)$(SIZE) $@ | $(PYTHON) $(TOP)/tools/build_memory_info.py $(LINKERFILE) $(BUILD)


# *******************************************************************************
### CKTPY BUILD RULES ###
include $(TOP)/py/mkrules.mk

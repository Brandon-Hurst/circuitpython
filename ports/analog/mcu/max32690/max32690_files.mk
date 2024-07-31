###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################

# This is the name of the build output file

# Define some macros for the Peripheral Drivers to compile properly
CFLAGS+=-DTARGET_REV=0x4131 -DTARGET=MAX32690 -DIAR_PRAGMAS=0

PERIPH_DIR := $(LIBS_DIR)/PeriphDrivers
SOURCE_DIR := $(PERIPH_DIR)/Source
INCLUDE_DIR := $(PERIPH_DIR)/Include

INC  += $(INCLUDE_DIR)/$(TARGET_UC)/

# Source files
SRC_C += $(SOURCE_DIR)/SYS/mxc_assert.c
SRC_C += $(SOURCE_DIR)/SYS/mxc_delay.c
SRC_C += $(SOURCE_DIR)/SYS/mxc_lock.c
SRC_C += $(SOURCE_DIR)/SYS/nvic_table.c
SRC_C += $(SOURCE_DIR)/SYS/pins_me18.c
SRC_C += $(SOURCE_DIR)/SYS/sys_me18.c


INC += -I$(SOURCE_DIR)/ADC
SRC_C += $(SOURCE_DIR)/ADC/adc_me18.c
SRC_C += $(SOURCE_DIR)/ADC/adc_revb.c

INC += -I$(SOURCE_DIR)/CAN
SRC_C += $(SOURCE_DIR)/CAN/can_me18.c
SRC_C += $(SOURCE_DIR)/CAN/can_reva.c

INC += -I$(SOURCE_DIR)/CTB
SRC_C += $(SOURCE_DIR)/CTB/ctb_me18.c
SRC_C += $(SOURCE_DIR)/CTB/ctb_reva.c
SRC_C += $(SOURCE_DIR)/CTB/ctb_common.c



INC += -I$(SOURCE_DIR)/DMA
SRC_C += $(SOURCE_DIR)/DMA/dma_me18.c
SRC_C += $(SOURCE_DIR)/DMA/dma_reva.c

INC += -I$(SOURCE_DIR)/EMCC
SRC_C += $(SOURCE_DIR)/EMCC/emcc_me18.c
SRC_C += $(SOURCE_DIR)/EMCC/emcc_reva.c

INC += -I$(SOURCE_DIR)/FLC
SRC_C += $(SOURCE_DIR)/FLC/flc_common.c
SRC_C += $(SOURCE_DIR)/FLC/flc_me18.c
SRC_C += $(SOURCE_DIR)/FLC/flc_reva.c

INC += -I$(SOURCE_DIR)/GPIO
SRC_C += $(SOURCE_DIR)/GPIO/gpio_common.c
SRC_C += $(SOURCE_DIR)/GPIO/gpio_me18.c
SRC_C += $(SOURCE_DIR)/GPIO/gpio_reva.c

INC += -I$(SOURCE_DIR)/HPB
SRC_C += $(SOURCE_DIR)/HPB/hpb_me18.c
SRC_C += $(SOURCE_DIR)/HPB/hpb_reva.c

INC += -I$(SOURCE_DIR)/I2C
SRC_C += $(SOURCE_DIR)/I2C/i2c_me18.c
SRC_C += $(SOURCE_DIR)/I2C/i2c_reva.c

INC += -I$(SOURCE_DIR)/I2S
SRC_C += $(SOURCE_DIR)/I2S/i2s_me18.c
SRC_C += $(SOURCE_DIR)/I2S/i2s_reva.c

INC += -I$(SOURCE_DIR)/ICC
SRC_C += $(SOURCE_DIR)/ICC/icc_me18.c
SRC_C += $(SOURCE_DIR)/ICC/icc_reva.c

INC += -I$(SOURCE_DIR)/LPCMP
SRC_C += $(SOURCE_DIR)/LPCMP/lpcmp_me18.c
SRC_C += $(SOURCE_DIR)/LPCMP/lpcmp_reva.c

INC += -I$(SOURCE_DIR)/OWM
SRC_C += $(SOURCE_DIR)/OWM/owm_me18.c
SRC_C += $(SOURCE_DIR)/OWM/owm_reva.c

INC += -I$(SOURCE_DIR)/PT
SRC_C += $(SOURCE_DIR)/PT/pt_me18.c
SRC_C += $(SOURCE_DIR)/PT/pt_reva.c

INC += -I$(SOURCE_DIR)/RTC
SRC_C += $(SOURCE_DIR)/RTC/rtc_me18.c
SRC_C += $(SOURCE_DIR)/RTC/rtc_reva.c

INC += -I$(SOURCE_DIR)/SEMA
SRC_C += $(SOURCE_DIR)/SEMA/sema_me18.c
SRC_C += $(SOURCE_DIR)/SEMA/sema_reva.c

INC += -I-I$(SOURCE_DIR)/SPI
SRC_C += $(SOURCE_DIR)/SPI/spi_me18.c
SRC_C += $(SOURCE_DIR)/SPI/spi_reva1.c

INC += -I$(SOURCE_DIR)/SPIXF
SRC_C += $(SOURCE_DIR)/SPIXF/spixf_me18.c
SRC_C += $(SOURCE_DIR)/SPIXF/spixf_reva.c

INC += -I$(SOURCE_DIR)/SPIXR
SRC_C += $(SOURCE_DIR)/SPIXR/spixr_me18.c
SRC_C += $(SOURCE_DIR)/SPIXR/spixr_reva.c

INC += -I$(SOURCE_DIR)/TRNG
SRC_C += $(SOURCE_DIR)/TRNG/trng_me18.c
SRC_C += $(SOURCE_DIR)/TRNG/trng_revb.c

INC += -I$(SOURCE_DIR)/TMR
SRC_C += $(SOURCE_DIR)/TMR/tmr_common.c
SRC_C += $(SOURCE_DIR)/TMR/tmr_me18.c
SRC_C += $(SOURCE_DIR)/TMR/tmr_revb.c

INC += -I$(SOURCE_DIR)/UART
SRC_C += $(SOURCE_DIR)/UART/uart_common.c
SRC_C += $(SOURCE_DIR)/UART/uart_me18.c
SRC_C += $(SOURCE_DIR)/UART/uart_revb.c

INC += -I$(SOURCE_DIR)/WDT
SRC_C += $(SOURCE_DIR)/WDT/wdt_common.c
SRC_C += $(SOURCE_DIR)/WDT/wdt_me18.c
SRC_C += $(SOURCE_DIR)/WDT/wdt_revb.c

INC += -I$(SOURCE_DIR)/WUT
SRC_C += $(SOURCE_DIR)/WUT/wut_me18.c
SRC_C += $(SOURCE_DIR)/WUT/wut_reva.c

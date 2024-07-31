/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/**
 * @file port.c
 * @author Brandon Hurst @ Analog Devices, Inc.
 * @brief Functions required for a basic CircuitPython port
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2024
 */

#include <stdint.h>
#include "supervisor/board.h"
#include "supervisor/port.h"

#include <stdio.h>
#include "gpio.h"
#include "led.h"
#include "mxc_assert.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "mxc_sys.h"
#include "pb.h"
#include "uart.h"

/** Linker variables defined....
 *  _estack:    end of the stack
 * _ebss:       end of BSS section
 * _ezero:      same as ebss (acc. to main.c)
 */
extern uint32_t _ezero;
extern uint32_t _estack;
extern uint32_t _ebss; // Stored at the end of the bss section (which includes the heap).


safe_mode_t port_init(void) {
    int err;

    // Initialize SysTick
    MXC_SYS_ClockEnable();

    // Enable GPIO / Periph clocks
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO0);
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO1);

    if ((err = PB_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    if ((err = LED_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    return SAFE_MODE_NONE;
}

void reset_cpi(void) {
    NVIC_SystemReset();
}

void reset_port(void) {
    MXC_GPIO_Reset(MXC_GPIO0);
    MXC_GPIO_Reset(MXC_GPIO1);
}

__attribute__((used)) void MemManage_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void BusFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void UsageFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

__attribute__((used)) void HardFault_Handler(void) {
    reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
    while (true) {
        asm ("nop;");
    }
}

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
// #include "gpio.h"
#include "mxc_assert.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "mxc_sys.h"
#include "uart.h"

/** Linker variables defined....
 *  _estack:    end of the stack
 * _ebss:       end of BSS section
 * _ezero:      same as ebss (acc. to main.c)
 */
extern uint32_t _ezero;
extern uint32_t _estack;
extern uint32_t _ebss; // Stored at the end of the bss section (which includes the heap).
extern uint32_t _ld_heap_start, _ld_heap_end, _ld_stack_top, _ld_stack_bottom;

// defined by cmsis core files
void NVIC_SystemReset(void) NORETURN;

safe_mode_t port_init(void) {
    return SAFE_MODE_NONE;
}

void HAL_Delay(uint32_t delay_ms) {
}

uint32_t HAL_GetTick(void) {
    return 1000;
}

void SysTick_Handler(void) {
}

void reset_to_bootloader(void) {
    NVIC_SystemReset();
}


void reset_cpu(void) {
    NVIC_SystemReset();
}

void reset_port(void) {
    // MXC_GPIO_Reset(MXC_GPIO0);
    // MXC_GPIO_Reset(MXC_GPIO1);
}

uint32_t *port_heap_get_bottom(void) {
    return (uint32_t *)0xAAAAAAAA;
}

uint32_t *port_heap_get_top(void) {
    return (uint32_t *)0xAAAAAAAF;
}

uint32_t *port_stack_get_limit(void) {
    #pragma GCC diagnostic push

    #pragma GCC diagnostic ignored "-Warray-bounds"
    // return port_stack_get_top() - (CIRCUITPY_DEFAULT_STACK_SIZE + CIRCUITPY_EXCEPTION_STACK_SIZE) / sizeof(uint32_t);
    return port_stack_get_top() - (0 + 0) / sizeof(uint32_t);
    #pragma GCC diagnostic pop
}

uint32_t *port_stack_get_top(void) {
    return (uint32_t *)0xB000000;
}


// Place the word to save just after our BSS section that gets blanked.
void port_set_saved_word(uint32_t value) {
    _ebss = value;
}

uint32_t port_get_saved_word(void) {
    return _ebss;
}

// __attribute__((used)) void MemManage_Handler(void) {
//     reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
//     while (true) {
//         asm ("nop;");
//     }
// }

// __attribute__((used)) void BusFault_Handler(void) {
//     reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
//     while (true) {
//         asm ("nop;");
//     }
// }

// __attribute__((used)) void UsageFault_Handler(void) {
//     reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
//     while (true) {
//         asm ("nop;");
//     }
// }

// __attribute__((used)) void HardFault_Handler(void) {
//     reset_into_safe_mode(SAFE_MODE_HARD_FAULT);
//     while (true) {
//         asm ("nop;");
//     }
// }

uint64_t port_get_raw_ticks(uint8_t *subticks) {
    return 1000;
}

// Enable 1/1024 second tick.
void port_enable_tick(void) {
}

// Disable 1/1024 second tick.
void port_disable_tick(void) {
}

void port_interrupt_after_ticks(uint32_t ticks) {
    // todo: implement isr after rtc ticks
}

void port_idle_until_interrupt(void) {
    __WFI();
}

// Required by __libc_init_array in startup code if we are compiling using
// -nostdlib/-nostartfiles.
void _init(void) {
}

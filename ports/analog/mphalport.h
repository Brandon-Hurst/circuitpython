// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Brandon Hurst
//
// SPDX-License-Identifier: MIT

#pragma once

#include "py/obj.h"
#include "lib/oofatfs/ff.h"
#include "supervisor/shared/tick.h"

void mp_hal_disable_all_interrupts(void);
void mp_hal_enable_all_interrupts(void);



#include <math.h>
#include "py/runtime.h"

#if CIRCUITPY_ALARM
#include "common-hal/alarm/__init__.h"
#endif

#include "common-hal/microcontroller/Processor.h"
#include "shared-bindings/microcontroller/ResetReason.h"

#include "system_max32690.h"

//
float common_hal_mcu_processor_get_temperature(void) {
    return NAN;
}

// TODO: Determine if there's a means of getting core voltage
float common_hal_mcu_processor_get_voltage(void) {
    return NAN;
}

uint32_t common_hal_mcu_processor_get_frequency(void) {
    return SystemCoreClock;
}

void common_hal_mcu_processor_get_uid(uint8_t raw_id[]) {
    return;
}

// TODO: May need to add reset reason in alarm / deepsleep cases
mcu_reset_reason_t common_hal_mcu_processor_get_reset_reason(void) {
    return RESET_REASON_UNKNOWN;
}

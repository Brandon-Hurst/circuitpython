
#include "mphalport.h"
#include "cmsis_gcc.h"


// TODO: Define tick & other port functions


void mp_hal_disable_all_interrupts(void) {
    __disable_irq();
}

void mp_hal_enable_all_interrupts(void) {
    __enable_irq();
}

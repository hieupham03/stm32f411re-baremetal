#include "bsp.h"
void BSP_Init(void) {

    SysTick_Init(1000000000);
    GPIO_InitLED();
}

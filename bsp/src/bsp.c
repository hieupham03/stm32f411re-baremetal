#include "bsp.h"
void BSP_Init(void) {

    SysTick_Init(100000000); // 100MHz system clock
    GPIO_InitLED();
    Button_Init();

}

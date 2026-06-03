#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>

void GPIO_InitLED(void);
void GPIO_WriteLED(uint8_t state);
void GPIO_ToggleLED(void);

#endif // BSP_GPIO_H

#include "bsp.h"
#include <stddef.h>

volatile uint8_t g_mode = 0;

int main(void) {
    BSP_Init();
    uint32_t last_toggle = 0;
    while (1) {
        if (g_mode == 0) {
            if (get_tick() - last_toggle >= 500) {
                GPIO_ToggleLED();
                last_toggle = get_tick();
            }
        } 
        else {
            GPIO_WriteLED(0);
        }

    }
    return 0;
}

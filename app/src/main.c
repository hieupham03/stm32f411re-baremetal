#include "bsp.h"

int main(void) {
    BSP_Init();
    uint32_t last_toggle = 0;
    while (1) {
        if (get_tick()-last_toggle >= 500) {
            GPIO_ToggleLED();
            last_toggle = get_tick();
        }
    }
    return 0;
}

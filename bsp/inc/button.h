#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include <stdint.h>


typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED  = 1
} Button_State_t;

void Button_Init(void);

Button_State_t Button_ReadState(void);

#endif // BSP_BUTTON_H

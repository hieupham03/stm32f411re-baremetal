#include <stdint.h>

extern uint32_t _estack;  
extern uint32_t _sidata;  
extern uint32_t _sdata;   
extern uint32_t _edata;  
extern uint32_t _sbss;   
extern uint32_t _ebss;  

extern int main(void);
void Reset_Handler(void);

void Default_Handler(void) {
    while(1);
}


__attribute__((section(".isr_vector")))
uint32_t* isr_vector[] = {
    (uint32_t*)&_estack,              
    (uint32_t*)Reset_Handler,         
    (uint32_t*)Default_Handler,     
    (uint32_t*)Default_Handler,     
};

void Reset_Handler(void) {
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;

    while (pDst < &_edata) {
        *pDst++ = *pSrc++;
    }

    pDst = &_sbss;
    while (pDst < &_ebss) {
        *pDst++ = 0;
    }
    main();

    while(1);
}
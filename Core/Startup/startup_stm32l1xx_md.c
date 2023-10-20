#include <stdint.h>
#include "system_stm32l1xx.h"
#include "stm32l1xx.h"
/* start address for the initialization values of the .data section. 
defined in linker script */
extern uint32_t _sidata;
/* start address for the .data section. defined in linker script */  
extern uint32_t _sdata;
/* end address for the .data section. defined in linker script */
extern uint32_t _edata;
/* start address for the .bss section. defined in linker script */
extern uint32_t _sbss;
/* end address for the .bss section. defined in linker script */
extern uint32_t _ebss;

extern uint32_t _estack;

extern void __libc_init_array();
extern int  main();
extern void Led_Life_Init();
extern void Debug_Init();
/**
 *          This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
*/

void Reset_Handler(){

    /* Copy the data segment initializers from flash to SRAM */
    uint32_t *dataInit = &_sidata;
    uint32_t *data     = &_sdata;
    while (data < &_edata){
        *data++ = *dataInit++;
    }
    /* Zero fill the bss segment. */  
    uint32_t *bss = &_sbss;
    while (bss < &_ebss){
        *bss++ = 0;
    }
    /* Call the clock system intitialization function.*/
    SystemInit();

    /* Call static constructors */
    __libc_init_array();
    
    /* Call my system intitialization function.*/
    Led_Life_Init();
    Debug_Init();
    /* Call the application's entry point.*/
    main();
}

/**
 *         This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.    
*/

void Default_Handler(){
    while(1);
}

void NMI_Handler(void)                     __attribute__ ((weak));
void HardFault_Handler(void)               __attribute__ ((weak));
void MemManage_Handler(void)               __attribute__ ((weak));
void BusFault_Handler(void)                __attribute__ ((weak));
void UsageFault_Handler(void)              __attribute__ ((weak));
void SVC_Handler(void)                     __attribute__ ((weak));
void DebugMon_Handler(void)                __attribute__ ((weak));
void PendSV_Handler(void)                  __attribute__ ((weak));
void SysTick_Handler(void)                 __attribute__ ((weak));
void USART2_IRQHandler(void)               __attribute__ ((weak));

__attribute__((section(".isr_vector")))
void (* const isr_vector[])() = {
    ((void (*)())(uint32_t)&_estack),
    Reset_Handler,         //Reset
    NMI_Handler,           //Non maskable interrupt.
    HardFault_Handler,     //All class of fault
    MemManage_Handler,     //Memory management
    BusFault_Handler,      //Pre-fetch fault, memory access fault
    UsageFault_Handler,    //Undefined instruction or illegal state
    Default_Handler,       //Reserved
    Default_Handler,       //Reserved
    Default_Handler,       //Reserved
    Default_Handler,       //Reserved
    SVC_Handler,           //System service call via SWI instruction
    DebugMon_Handler,      //Debug Monitor
    Default_Handler,       //Reserved
    PendSV_Handler,        //Pendable request for system service
    SysTick_Handler,       //System tick timer
    Default_Handler,       //Window Watchdog interrupt
    Default_Handler,       //PVD through EXTI Line detection interrupt
    Default_Handler,       //Tamper and TimeStamp through EXTI line interrupts
    Default_Handler,       //RTC Wakeup through EXTI line interrupt
    Default_Handler,       //Flash global interrupt
    Default_Handler,       //RCC global interrupt
    Default_Handler,       //EXTI Line0 interrupt
    Default_Handler,       //EXTI Line1 interrupt
    Default_Handler,       //EXTI Line2 interrupt
    Default_Handler,       //EXTI Line3 interrupt
    Default_Handler,       //EXTI Line4 interrupt
    Default_Handler,       //DMA1 Channel1 global interrupt
    Default_Handler,       //DMA1 Channel2 global interrupt
    Default_Handler,       //DMA1 Channel3 global interrupt
    Default_Handler,       //DMA1 Channel4 global interrupt
    Default_Handler,       //DMA1 Channel5 global interrupt
    Default_Handler,       //DMA1 Channel6 global interrupt
    Default_Handler,       //DMA1 Channel7 global interrupt
    Default_Handler,       //ADC1 global interrupt
    Default_Handler,       //USB High priority interrupt
    Default_Handler,       //USB Low priority interrupt
    Default_Handler,       //DAC interrupt
    Default_Handler,       //Comparator wakeup through EXTI line (21 and 22) interrupt, touch sense interrupt(1)
    Default_Handler,       //EXTI Line[9:5] interrupts
    Default_Handler,       //LCD global interrupt
    Default_Handler,       //TIM9 global interrupt
    Default_Handler,       //TIM10 global interrupt
    Default_Handler,       //TIM11 global interrupt
    Default_Handler,       //TIM2 global interrupt
    Default_Handler,       //TIM3 global interrupt
    Default_Handler,       //TIM4 global interrupt
    Default_Handler,       //I2C1 event interrupt
    Default_Handler,       //I2C1 error interrupt
    Default_Handler,       //I2C2 event interrupt
    Default_Handler,       //I2C2 error interrupt
    Default_Handler,       //SPI1 global interrupt
    Default_Handler,       //SPI2 global interrupt
    Default_Handler,       //USART1 global interrupt
    USART2_IRQHandler,     //USART2 global interrupt
    Default_Handler,       //USART3 global interrupt
    Default_Handler,       //EXTI Line[15:10] interrupts
    Default_Handler,       //RTC Alarms (A and B) through EXTI line interrupt
    Default_Handler,       //USB Device FS Wakeup through EXTI line interrupt
    Default_Handler,       //TIM6 global interrupt
    Default_Handler        //TIM7 global interrupt
};
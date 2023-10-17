


#ifndef _LEDLIFE_H_
#define _LEDLIFE_H_
#include "stm32l1xx_conf.h"

#ifdef __cplusplus
      extern "C"
#endif

//  Pin map led life
#define LED_LIFE_IO_PIN					(GPIO_Pin_8)
#define LED_LIFE_IO_PORT				(GPIOB)
#define LED_LIFE_IO_CLOCK				(RCC_AHBPeriph_GPIOB)

extern void Led_Life_Init();
void Led_Life_Toggle();

#ifdef __cplusplus
#endif
#endif
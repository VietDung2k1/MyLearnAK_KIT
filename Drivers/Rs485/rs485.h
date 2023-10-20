#ifndef __RS485__
#define __RS485__
#include "stm32l1xx_conf.h"
#include <stdint.h>
#if defined (__cplusplus)
extern "C" {
#endif

/****************************************************************************
 *Pin map UART2
*****************************************************************************/
#define USART2_TX_PIN					GPIO_Pin_3
#define USART2_TX_GPIO_PORT				GPIOA
#define USART2_TX_GPIO_CLK				RCC_AHBPeriph_GPIOA
#define USART2_TX_SOURCE				GPIO_PinSource3
#define USART2_TX_AF					GPIO_AF_USART1

#define USART2_RX_PIN					GPIO_Pin_2
#define USART2_RX_GPIO_PORT				GPIOA
#define USART2_RX_GPIO_CLK				RCC_AHBPeriph_GPIOA
#define USART2_RX_SOURCE				GPIO_PinSource2
#define USART2_RX_AF					GPIO_AF_USART2

#define IRQ_PRIO_UART2_IO				3

#define USART2_CLK						RCC_APB1Periph_USART2
void Rs485_Init(uint32_t);


#ifdef __cplusplus
}
#endif

#endif
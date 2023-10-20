
#ifndef _DEBUG_H_
#define _DEBUG_H_
#include <stm32l1xx.h>
#include <stm32l1xx_conf.h>
#include "stm32l1xx_it.h"
#include "xprintf.h"
/* USART Communication boards Interface */

#define USART1_CLK                       RCC_APB2Periph_USART1
#define USART1_IRQn                      USART1_IRQn
#define USART1_IRQHandler                USART1_IRQHandler

#define USART1_TX_PIN                    GPIO_Pin_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define USART1_TX_SOURCE                 GPIO_PinSource9
#define USART1_TX_AF                     GPIO_AF_USART1

#define USART1_RX_PIN                    GPIO_Pin_10
#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define USART1_RX_SOURCE                 GPIO_PinSource10
#define USART1_RX_AF                     GPIO_AF_USART1
#define SYS_CONSOLE_BAUDRATE             115200
#define IRQ_PRIO_UART0_CONSOLE	         2

extern void Debug_Init();
void Redirect_Printf(uint8_t c);
void Delay_Ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif
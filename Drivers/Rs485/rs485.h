#ifndef __RS485__
#define __RS485__
#include "stm32l1xx_conf.h"
#include <stdint.h>
#include "debug.h"
#include "modbus_slave.h"
#if defined (__cplusplus)
extern "C" {
#endif


/*RS485 dir IO*/
#define RS485_DIR_IO_PIN				(GPIO_Pin_1)
#define RS485_DIR_IO_PORT				(GPIOA)
#define RS485_DIR_IO_CLOCK				(RCC_AHBPeriph_GPIOA)

/****************************************************************************
 *Pin map UART2
*****************************************************************************/
#define RS485_TX_PIN					GPIO_Pin_3
#define RS485_TX_GPIO_PORT				GPIOA
#define RS485_TX_GPIO_CLK				RCC_AHBPeriph_GPIOA
#define RS485_TX_SOURCE				    GPIO_PinSource3
#define RS485_TX_AF					    GPIO_AF_USART1

#define RS485_RX_PIN					GPIO_Pin_2
#define RS485_RX_GPIO_PORT				GPIOA
#define RS485_RX_GPIO_CLK				RCC_AHBPeriph_GPIOA
#define RS485_RX_SOURCE				    GPIO_PinSource2
#define RS485_RX_AF					    GPIO_AF_USART2

#define IRQ_PRIO_RS485_IO				3

#define RS485_CLK						RCC_APB1Periph_USART2
#define buffer_rx_size                  10
void Rs485_Init(uint32_t);
void Rs485_SendData(USART_TypeDef*, uint8_t*, uint16_t);
void Rs485_ReadData();
#ifdef __cplusplus
}
#endif

#endif
#include "rs485.h"
#include "debug.h"

uint8_t RxData[buffer_rx_size];
uint8_t TxData[buffer_tx_size];
uint8_t countRx;
extern uint16_t SLAVE_ID;
uint8_t rx_index = 0;
void Rs485_Init(uint32_t BaundRate){

	/*RS485 dir IO*/
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RS485_DIR_IO_CLOCK, ENABLE);
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin		= RS485_DIR_IO_PIN;
	GPIO_Init(RS485_DIR_IO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN);

	/* UART2 */
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RS485_TX_GPIO_CLK | RS485_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RS485_CLK, ENABLE);

	/* Connect PXx to USART2_Tx */
	GPIO_PinAFConfig(RS485_TX_GPIO_PORT, RS485_TX_SOURCE, RS485_TX_AF);

	/* Connect PXx to USART2_Rx */
	GPIO_PinAFConfig(RS485_RX_GPIO_PORT, RS485_RX_SOURCE, RS485_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = RS485_TX_PIN;
	GPIO_Init(RS485_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RS485_RX_PIN;
	GPIO_Init(RS485_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USART2 configuration */
	USART_InitStructure.USART_BaudRate = BaundRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = IRQ_PRIO_RS485_IO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USART2, USART_IT_RXNE | USART_IT_TXE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
    
}

void Rs485_SendData(USART_TypeDef* USARTx, uint8_t *data, uint16_t size){
    GPIO_SetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN);
	size+=1;
	while (size--) {
        while (!(USARTx->SR & USART_SR_TXE));
		// xprintf("Send Data %d : 0x%x\n",size, *data);
        USART_SendData(USARTx, *data++);
    }
	while (!(USARTx->SR & USART_SR_TXE));
	GPIO_ResetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN);
}
void Rs485_ReadData(){
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // Read data from the receive buffer and process it
        uint8_t data = USART_ReceiveData(USART2);
        if (rx_index < buffer_rx_size) {
            RxData[rx_index++] = data;
            // Process the data here if needed
			// xprintf("RxData[%d] : 0x%x\n", rx_index-1, RxData[rx_index-1]);
            // Clear the RXNE interrupt
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        }
    } else if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        // Handle the case when there is no more incoming data
        // Reset rx_index to 0
		// Rs485_SendData(USART2, RxData, rx_index);
		for (countRx = 0; countRx < buffer_rx_size; countRx++)
		{
			if (RxData[countRx] == SLAVE_ID)
				{
					switch (RxData[countRx+1]){
					case 0x03:
						readHoldingRegs();
						break;
					case 0x04:
						readInputRegs();
						break;
					case 0x06:
						writeSingerRegs();
						break;
					default:
						break;
					}
				}
			// xprintf("--RxData[%d] = 0x%x\n", countRx, RxData[countRx]);
		}
		rx_index = 0;
        // Clear the IDLE interrupt
        USART_ReceiveData(USART2); // Read one byte to clear the IDLE flag
    }
}
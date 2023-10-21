

#include "debug.h"

static __IO uint32_t TimingDelay;

void Debug_Init()
{

    // SysTick Config
    volatile uint32_t ticks = SystemCoreClock / 1000;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	SysTick->LOAD  = ticks - 1;                                  /* set reload register */

	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(SysTick_IRQn, 1);

	SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_TICKINT_Msk   |
			SysTick_CTRL_ENABLE_Msk;                             /* Enable SysTick IRQ and SysTick Timer */
    
    
    // Usart1 config
    USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// ring_buffer_char_init(&ring_buffer_char_shell_send, ring_buffer_char_shell_send_buffer, RING_BUFFER_CHAR_SHELL_SEND_BUFFER_SIZE);

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(USART1_TX_GPIO_CLK | USART1_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);

	/* Connect PXx to USART1_Tx */
	GPIO_PinAFConfig(USART1_TX_GPIO_PORT, USART1_TX_SOURCE, USART1_TX_AF);

	/* Connect PXx to USART1_Rx */
	GPIO_PinAFConfig(USART1_RX_GPIO_PORT, USART1_RX_SOURCE, USART1_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USART1 configuration */
	USART_InitStructure.USART_BaudRate = SYS_CONSOLE_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	xfunc_output = (void(*)(int))Redirect_Printf;


	xprintf("-----------------------\n");
	xprintf("---!!MODBUS SENSOR!!---\n");
	xprintf("-----------------------\n");
	xprintf("--Temp       : 0x00 : Read\n");
	xprintf("--Hum        : 0x01 : Read\n");
	xprintf("--Id         : 0x1E : R/W\n");
	xprintf("--Baudrate   : 0x1F : R/W\n");
	xprintf("-----------------------\n");
	xprintf("\n");
}

void Redirect_Printf(uint8_t c) {
	/* wait last transmission completed */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	/* put transnission data */
	USART_SendData(USART1, (uint8_t)c);

	/* wait transmission completed */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void Delay_Ms(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}
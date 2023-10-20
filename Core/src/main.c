
#include "main.h"

uint8_t RxData[256];
uint8_t TxData[256];
uint8_t countRx;

extern uint16_t SLAVE_ID;
extern uint32_t BAUND_RATE;

int main(){	

	if (modbus_slave_init() == MODBUS_SLAVE_OK){
		xprintf("---modbus_slave_init--- PASS\n");
	} else {
		xprintf("---modbus_slave_init--- FALSE\n");
	}
	
	Rs485_Init(BAUND_RATE);
	xprintf("---Setup Rs485 Id : %d ", SLAVE_ID);
	xprintf("---Baudrate : %d\n", BAUND_RATE);

	

    while(1){
		Led_Life_Toggle();
      	Delay_Ms(1000);
    }
}




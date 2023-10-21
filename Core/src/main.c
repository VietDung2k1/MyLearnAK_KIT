
#include "main.h"
#include <stdlib.h>
extern uint16_t SLAVE_ID;
extern uint32_t BAUND_RATE;
extern uint8_t rx_index;
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
		rx_index = 0;
		uint16_t random_temp = (uint16_t)rand() % 301 + 100;
		uint16_t random_hum  = (uint16_t)rand() % 301 + 600;
		update_sensor_Registers(random_temp, random_hum);
		Led_Life_Toggle();
		Delay_Ms(1000);
    }
}




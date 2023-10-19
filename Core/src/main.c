
#include "main.h"

int main(){
	
	uint8_t DataWrite = 0x15;
	uint8_t DataRead;

	// if (eeprom_write(0, &DataWrite, 1) == EEPROM_DRIVER_OK){
	// 	xprintf("Write Data : 0x08008 0000\n");
	// }

    while(1){
		if (eeprom_read(0, &DataRead, 1)   == EEPROM_DRIVER_OK){
			xprintf("Read Data : 0x08008 0000 == %x \n", DataRead);
		}
		Led_Life_Toggle();
      	Delay_Ms(1000);
    }
}

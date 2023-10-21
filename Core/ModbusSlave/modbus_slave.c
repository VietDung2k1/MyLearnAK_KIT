

#include "modbus_slave.h"

extern uint8_t RxData[buffer_rx_size];
extern uint8_t TxData[buffer_tx_size];
extern uint8_t countRx;

uint16_t SLAVE_ID;
uint32_t BAUND_RATE;

uint8_t modbus_slave_init(){
    uint8_t id_fr_eeprom;
    if (eeprom_read(0, &id_fr_eeprom, 1) == EEPROM_DRIVER_OK){
        xprintf("Read id EEPROM 0x0808 0000 = %x\n", id_fr_eeprom);
        Holding_Registers_Database[30] = (uint16_t)id_fr_eeprom;
        xprintf("Write id Holding_Registers_Database[30] = %x\n", Holding_Registers_Database[30]);
    } else {
        xprintf("Read id EEPROM 0x0808 0000 : FALSE\n");
        return MODBUS_SLAVE_FALSE;
    }   
    SLAVE_ID = Holding_Registers_Database[30];
    xprintf("SLAVE_ID : %x\n", SLAVE_ID);
    uint8_t baudrate_fr_eeprom;
    if (eeprom_read(1, &baudrate_fr_eeprom, 1) == EEPROM_DRIVER_OK){
        xprintf("Read baudrate EEPROM 0x0808 0001 = %x\n", baudrate_fr_eeprom);        
        Holding_Registers_Database[31] = (uint16_t)baudrate_fr_eeprom;
        xprintf("Write baudrate Holding_Registers_Database[31] = %x\n", Holding_Registers_Database[31]);        

    } else {
        xprintf("Read baudrate EEPROM 0x0808 0001 : FALSE\n");
        return MODBUS_SLAVE_FALSE;
    }  

    switch (Holding_Registers_Database[31])
    {
    case 0:                                        //If 40032 = 0, baud rate = 4800
        BAUND_RATE = 4800;
        break;
    case 1:                                        //If 40032 = 1, baud rate = 9600
        BAUND_RATE = 9600;
        break;
    case 2:                                        //If 40032 = 2, baud rate = 115200
        BAUND_RATE = 115200;
        break;
    default:                                       //If 40032 != 0|1|2, baud rate = 9600 
        BAUND_RATE = 9600;
        break;
    }
    xprintf("BAUND_RATE : %d\n", BAUND_RATE);
    return MODBUS_SLAVE_OK;
}

uint8_t update_sensor_Registers(uint16_t Temp, uint16_t Hum){

    if(Temp != Holding_Registers_Database[0]){
        Holding_Registers_Database[0] = Temp;
    }

    if(Hum != Holding_Registers_Database[1]){
        Holding_Registers_Database[1] = Hum;
    }
}

uint8_t update_Id_Registers(uint16_t id){
    if(id != Holding_Registers_Database[30]){
        if (id >= 1 && id <= 254){
            uint8_t id_eep_buf = (uint8_t)id;
            if (eeprom_write(0, &id_eep_buf, 1) == EEPROM_DRIVER_OK){
                //wait for the user to reboot
                while (1)
                {
                    modbusException(ILLEGAL_DATA_ADDRESS);
                    Delay_Ms(1000);
                }
            }
        } else {
            modbusException(ILLEGAL_DATA_VALUE);
        }
    }
}
uint8_t update_Baudrate_Registers(uint16_t baudrate){
    if(baudrate != Holding_Registers_Database[31]){
        if (baudrate >= 0 && baudrate <= 2){
            uint8_t baudrate_eep_buf = (uint8_t)baudrate;
            if (eeprom_write(1, &baudrate_eep_buf, 1) == EEPROM_DRIVER_OK){
                //wait for the user to reboot
                while (1)
                {
                    modbusException(ILLEGAL_FUNCTION);
                    Delay_Ms(1000);
                }
            }
        } else {
            modbusException(ILLEGAL_DATA_VALUE);
        }
    }
}

void sendData (uint8_t *data, int size)
{
	// we will calculate the CRC in this function itself
	uint16_t crc = crc16(data, size);
	data[size] = crc&0xFF;   // CRC LOW
	data[size+1] = (crc>>8)&0xFF;  // CRC HIGH
    
    Rs485_SendData(USART2, data, size+2);
}

void modbusException (uint8_t exceptioncode)
{
	//| SLAVE_ID | FUNCTION_CODE | Exception code | CRC     |
	//| 1 BYTE   |  1 BYTE       |    1 BYTE      | 2 BYTES |

	TxData[0] = RxData[countRx];       // slave ID
	TxData[1] = RxData[countRx+1]|0x80;  // adding 1 to the MSB of the function code
	TxData[2] = exceptioncode;   // Load the Exception code
	sendData(TxData, 3);         // send Data... CRC will be calculated in the function
}


uint8_t readHoldingRegs (void)
{
	uint16_t startAddr = ((RxData[countRx+2]<<8)|RxData[countRx+3]);  // start Register Address

	uint16_t numRegs = ((RxData[countRx+4]<<8)|RxData[countRx+5]);   // number to registers master has requested
	if ((numRegs<1)||(numRegs>125))  // maximum no. of Registers as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numRegs-1;  // end Register
	if (endAddr>49)  // end Register can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	// Prepare TxData buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData[0] = SLAVE_ID;  // slave ID
	TxData[1] = RxData[countRx+1];  // function code
	TxData[2] = numRegs*2;  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData Buffer

	for (int i=0; i<numRegs; i++)   // Load the actual data into TxData buffer
	{
		TxData[indx++] = (Holding_Registers_Database[startAddr]>>8)&0xFF;  // extract the higher byte
		TxData[indx++] = (Holding_Registers_Database[startAddr])&0xFF;   // extract the lower byte
		startAddr++;  // increment the register address
	}

	sendData(TxData, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t readInputRegs (void)
{
	uint16_t startAddr = ((RxData[countRx+2]<<8)|RxData[countRx+3]);  // start Register Address

	uint16_t numRegs = ((RxData[countRx+4]<<8)|RxData[countRx+5]);   // number to registers master has requested
	if ((numRegs<1)||(numRegs>125))  // maximum no. of Registers as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numRegs-1;  // end Register
	if (endAddr>49)  // end Register can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	// Prepare TxData buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData[0] = SLAVE_ID;  // slave ID
	TxData[1] = RxData[countRx+1];  // function code
	TxData[2] = numRegs*2;  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData Buffer

	for (int i=0; i<numRegs; i++)   // Load the actual data into TxData buffer
	{
		TxData[indx++] = (Input_Registers_Database[startAddr]>>8)&0xFF;  // extract the higher byte
		TxData[indx++] = (Input_Registers_Database[startAddr])&0xFF;   // extract the lower byte
		startAddr++;  // increment the register address
	}

	sendData(TxData, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}


uint8_t writeSingerRegs (void)
{
	uint16_t Addr = ((RxData[countRx+2]<<8)|RxData[countRx+3]);  //Register Address

	uint16_t Value = ((RxData[countRx+4]<<8)|RxData[countRx+5]);   //Value write address 

	// Prepare TxData buffer

	//| SLAVE_ID | FUNCTION_CODE | REGISTER   | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | 1 BYTES   | 2 BYTES |

	TxData[0] = SLAVE_ID;  // slave ID
	TxData[1] = RxData[countRx+1];  // function code
	TxData[2] = RxData[countRx+2];  // Register high
    TxData[3] = RxData[countRx+3];  // Register low
    TxData[4] = RxData[countRx+4];  // Data high
    TxData[5] = RxData[countRx+5];  // Data low
    int indx = 6;
	sendData(TxData, indx);  // send data... CRC will be calculated in the function itself

    //Check write id and baudrate
    switch (Addr)
    {
    case 0x1E:
        update_Id_Registers(Value);
        break;
    case 0x1F:
        update_Baudrate_Registers(Value);
        break;
    default:
        break;
    }
	return 1;   // success
}


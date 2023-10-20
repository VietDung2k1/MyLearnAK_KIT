

#ifndef INC_MODBUS_SLAVE_H_
#define INC_MODBUS_SLAVE_H_

#include "modbus_crc.h"
#include "stm32l1xx_conf.h"
#include "eeprom.h"
#include <stdint.h>
#include "debug.h"
#include "rs485.h"
#define ILLEGAL_FUNCTION       0x01
#define ILLEGAL_DATA_ADDRESS   0x02
#define ILLEGAL_DATA_VALUE     0x03

#define MODBUS_SLAVE_OK        0x00
#define MODBUS_SLAVE_FALSE     0x01
#define buffer_tx_size 30
#define buffer_rx_size 10
/*
* 40001 : Temperature
* 40002 : Humidity
* 40003 : Temperature correction
* 40004 : Humidity correction
* 40031 : ID
* 40032 : Baund Rate
*/

static uint16_t Holding_Registers_Database[50]={
	 320,   742,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 0-9   40001-40010
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 10-19 40011-40020
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 20-29 40021-40030
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 30-39 40031-40040
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 40-49 40041-40050
}; 

static const uint16_t Input_Registers_Database[50]={
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 0-9   30001-30010
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 10-19 30011-30020
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 20-29 30021-30030
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 30-39 30031-30040
	0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  0000,  // 40-49 30041-30050
};

uint8_t modbus_slave_init();
uint8_t update_sensor_Registers(uint16_t, uint16_t);
uint8_t update_Registers(uint16_t, uint16_t);
uint8_t readHoldingRegs (void);
uint8_t readInputRegs (void);

#endif /* INC_MODBUS_SLAVE_H_ */

#ifndef __EEPROM_H__
#define __EEPROM_H__

#if defined (__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "stm32l1xx_flash.h"
/******************************************************************************
* eeprom function
*******************************************************************************/
#define EEPROM_BASE_ADDRESS         (0x08080000)
#define EEPROM_MAX_SIZE             (0x1000)

#define EEPROM_DRIVER_OK            (0x00)
#define EEPROM_DRIVER_NG            (0x01)

extern uint8_t eeprom_read (uint32_t address, uint8_t* pbuf, uint32_t len);
extern uint8_t eeprom_write(uint32_t address, uint8_t* pbuf, uint32_t len);
extern uint8_t eeprom_erase(uint32_t address, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif //__EEPROM_H__

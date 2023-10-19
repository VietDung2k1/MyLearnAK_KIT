#include "eeprom.h"

/******************************************************************************
* eeprom function
* when using function DATA_EEPROM_ProgramByte,
* must be DATA_EEPROM_unlock- DATA_EEPROM_ProgramByte- DATA_EEPROM_lock
*******************************************************************************/
uint8_t eeprom_read(uint32_t address, uint8_t* pbuf, uint32_t len) {
	uint32_t eeprom_address = 0;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if ((uint8_t*)pbuf == (uint8_t*)0 || len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();
	memcpy(pbuf, (const uint8_t*)eeprom_address, len);
	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

uint8_t eeprom_write(uint32_t address, uint8_t* pbuf, uint32_t len) {
	uint32_t eeprom_address = 0;
	uint32_t index = 0;
	uint32_t flash_status;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if ((uint8_t*)pbuf == (uint8_t*)0 || len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();

	while (index < len) {
		flash_status = DATA_EEPROM_ProgramByte((eeprom_address + index), (uint32_t)(*(pbuf + index)));

		if(flash_status == FLASH_COMPLETE) {
			index++;
		}
		else {
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
							| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
		}
	}

	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

uint8_t eeprom_erase(uint32_t address, uint32_t len) {
	uint32_t eeprom_address = 0;
	uint32_t index = 0;
	uint32_t flash_status;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if (len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();

	while(index < len) {
		flash_status = DATA_EEPROM_ProgramByte(eeprom_address + index, 0x00);

		if(flash_status == FLASH_COMPLETE) {
			index++;
		}
		else {
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
							| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
		}
	}

	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

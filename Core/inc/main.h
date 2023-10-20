#ifndef __MAIN_H
#define __MAIN_H
#include <stm32l1xx.h>
#include <stm32l1xx_conf.h>

#include "debug.h"
#include "led_life.h"
#include "eeprom.h"
#include "modbus_slave.h"
#include "rs485.h"
#ifdef __cplusplus
      extern "C"
#endif
void My_Gpio_Init();
void Usart1_Init();

#ifdef __cplusplus
#endif

#endif
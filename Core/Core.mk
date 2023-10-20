

CFLAGS += -ICore/inc \
-ICore/ModbusSlave

SRC += \
Core/src/stm32l1xx_it.c \
Core/src/main.c \
Core/ModbusSlave/modbus_slave.c \
Core/ModbusSlave/modbus_crc.c

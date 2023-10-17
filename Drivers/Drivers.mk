
CFLAGS += \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Device/ST/STM32L1xx/Include \
-IDrivers/STM32L1xx_StdPeriph_Driver/inc \
-IDrivers/Debug \
-IDrivers/LedLife

#CMSIS
SRC += Drivers/CMSIS/Device/ST/STM32L1xx/Source/Templates/system_stm32l1xx.c

#Debug
SRC += Drivers/Debug/xprintf.c \
Drivers/Debug/debug.c

#Led life
SRC += Drivers/LedLife/led_life.c

#STM32F4XX_HAL_DRIVER
SRC += \
Drivers/STM32L1xx_StdPeriph_Driver/src/misc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_adc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_aes_util.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_aes.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_comp.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_crc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_dac.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_dbgmcu.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_dma.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_exti.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_flash_ramfunc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_flash.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_fsmc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_gpio.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_i2c.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_iwdg.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_lcd.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_opamp.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_pwr.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_rcc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_rtc.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_pwr.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_sdio.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_spi.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_syscfg.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_tim.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_usart.c \
Drivers/STM32L1xx_StdPeriph_Driver/src/stm32l1xx_wwdg.c 















############################################################################
# Operating system								   
############################################################################

WINDOW  = 0
LINUX   = 1
#Select development environment
SYSTEM 	= WINDOW

############################################################################
# Verbosity								   
############################################################################
# Be silent per default, but 'make V=1' will show all compiler calls.
# If you're insane, V=99 will print out all sorts of things.
V?=0
ifeq ($(V),0)
Q	:= @
NULL	:= 2>/dev/null
endif

############################################################################
# Paths
############################################################################

# Build path
BUILD_DIR = build
# Default flashing tool is st-flash
FLASHING_TOOL ?= st-flash
FLASH  = 0x08000000

############################################################################
# Configure tools from the GCC ARM Embedded toolchain
############################################################################

# Gnu toolchain path
ifeq ($(SYSTEM),WINDOW)
PREFIX  = arm-none-eabi-
CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE    = $(PREFIX)size
endif

ifeq ($(SYSTEM),LINUX)
GNUTOOL = $(HOME)/workspace/tools/gcc-arm-none-eabi-10.3-2021.10/bin/
PREFIX  = $(GNUTOOL)arm-none-eabi-
CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE    = $(PREFIX)size
endif
ECHO = @echo

############################################################################
# Flags 
############################################################################

# Flags - Overall Options
CFLAGS += -specs=nosys.specs
CFLAGS += -std=gnu99

# C Libraries
CFLAGS += -lc -lm -lnosys

# Flags - C Language Options
CFLAGS += -ffreestanding

# Flags - Optimization Options
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections

# Flags - Debugging Options
CFLAGS += -g

# Flags - Preprocessor options
CFLAGS += -DSTM32L1XX_MD
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DDEBUG

# Flags - Assembler Options
CFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1

# Flags - Linker Options
CFLAGS += -TStm32l151_md.ld

# Flags - Machine-dependant options
CFLAGS += -mthumb
CFLAGS += -march=armv7-m
CFLAGS += -mfloat-abi=soft

MAPPED_DEVICE = STM32L151MD

# Output files
ELF_FILE_NAME ?= stm32_executable.elf
BIN_FILE_NAME ?= stm32_l151_app.bin
OBJ_FILE_NAME ?= startup_$(MAPPED_DEVICE).o

ELF_FILE_PATH = $(BUILD_DIR)/$(ELF_FILE_NAME)
BIN_FILE_PATH = $(BUILD_DIR)/$(BIN_FILE_NAME)
OBJ_FILE_PATH = $(BUILD_DIR)/$(OBJ_FILE_NAME)

# Input files
SRC ?=
include Drivers/Drivers.mk
include Core/Core.mk

# Startup file
DEVICE_STARTUP = Core/Startup/startup_stm32l1xx_md.c

# A simply expanded variable is used here
# to perform the find command only once.
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))
vpath %.c $(sort $(dir $(SRC)))

############################################################################
# build the application
############################################################################

# Make allS
all:$(BIN_FILE_PATH)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(ECHO) "CC      $<"
	$(Q)$(CC) -c $(CFLAGS) $< -o $@

$(BIN_FILE_PATH): $(ELF_FILE_PATH)
	$(ECHO) "SIZE __      $@"
	$(Q)$(SIZE) $<
	$(ECHO) "BINFILE      $@"
	$(Q)$(OBJCOPY) -O binary $^ $@
	
$(ELF_FILE_PATH): $(OBJECTS) $(OBJ_FILE_PATH)
	$(Q)$(CC) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	$(Q)$(SIZE) $@

$(OBJ_FILE_PATH): $(DEVICE_STARTUP)
	$(ECHO) "CC      $<"
	$(Q)$(CC) -c $(CFLAGS) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR):
	$(ECHO) "	##############################################################"
	$(ECHO) "	##                Modbus Sensor : VietDung2k1               ##"
	$(ECHO) "	##############################################################"
	$(ECHO) ""
	$(ECHO) ""
	$(Q)mkdir $@		

ifeq ($(SYSTEM), WINDOW)
# Make clean on window
clean:
	rmdir /s /q $(BUILD_DIR)
flash: all # Flash on window
	openocd -f openocd.cfg -c "program $(ELF_FILE_PATH) verify reset exit"
endif

ifeq ($(SYSTEM),LINUX)
# Make clean on linux
clean:
	rm -r $(BUILD_DIR)
flash: all # Flash on linux
ifeq ($(FLASHING_TOOL), st-flash)
	st-flash write $(BIN_FILE_PATH) $(FLASH)
else
	$(error $(FLASHING_TOOL) not recognized)
endif
endif

.PHONY: all clean flash


############################################################################
##                     MakeFile create by VietDungUtc2                    ##
############################################################################
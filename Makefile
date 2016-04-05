#define TARGET Name
TARGET=template

#define TOOLCHAIN
#CC_BASE_PATH=/home/harbio/sdk2/gcc-arm-none-eabi-5_3-2016q1
ifeq (,$(CC_BASE_PATH))
CC_BASE_PATH=/usr
endif
CC=$(CC_BASE_PATH)/bin/arm-none-eabi-gcc
AR=$(CC_BASE_PATH)/bin/arm-none-eabi-ar
LD=$(CC_BASE_PATH)/bin/arm-none-eabi-ld
OBJCOPY=$(CC_BASE_PATH)/bin/arm-none-eabi-objcopy


DEBUGGER=$(CC_BASE_PATH)/bin/arm-none-eabi-gdb
ifeq (,$(STLINK))
STLINK=/opt/STM/stlink
endif

#define Toolchain and Library paths
ifeq (,$(CUBE_FW_BASE))
CUBE_FW_BASE=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0
endif
ifeq (,$(CC_VERSION))
CC_VERSION=`ls $(CC_BASE_PATH)/lib/gcc/arm-none-eabi`
endif
LIB_PATH   ?=$(CC_BASE_PATH)/lib/gcc/arm-none-eabi/$(CC_VERSION)

HAL_PATH   ?=$(CUBE_FW_BASE)/Drivers/STM32F4xx_HAL_Driver
CMSIS_PATH ?=$(CUBE_FW_BASE)/Drivers/CMSIS
BSP_PATH   ?=$(CUBE_FW_BASE)/Drivers/BSP

FATFS_PATH ?=$(CUBE_FW_BASE)/Middlewares/Third_Party/fatafs_stm32f4

#define INCLUDES
BASE_INCLUDES= -I$(LIB_PATH)/include-fixed

HAL_INCLUDES= -I$(HAL_PATH)/Inc -I$(CMSIS_PATH)/Include -I$(CMSIS_PATH)/Device/ST/STM32F4xx/Include

CUSTOM_INCLUDES = -IInc -I$(BSP_PATH)/STM32F4-Discovery -I$(FATFS_PATH) -I$(FATFS_PATH)/drivers

CUSTOM_INCLUDES += -Iexternal_libs/molup

#define CFLAGS
DBG = -Os -fdata-sections -ffunction-sections -Wl,--gc-sections
ifneq (,$(DEBUG))
DBG = -g
endif

CFLAGS = -c $(DBG) -Wall -DUSE_STD_PERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=soft
CFLAGS += $(CUSTOM_INCLUDES) $(HAL_INCLUDES) $(BASE_INCLUDES)

CFLAGS += -DSTM32F4_DISCOVERY -DSTM32F407xx -DUSE_HAL_DRIVER

LDFLAGS = -L $(CC_BASE_PATH)/lib/gcc/arm-none-eabi/$(CC_VERSION)/armv7e-m -lgcc -L $(CC_BASE_PATH)/arm-none-eabi/lib/thumb -lc -TSTM32F407VGTx_FLASH.ld -Map $(TARGET).map -nostartfiles

#define SRC FILES
SRCS  = startup_stm32f407xx.s
SRCS += Src/stm32f4_discovery.c

SRCS += $(HAL_PATH)/Src/stm32f4xx_hal.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_cortex.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_gpio.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_rcc.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_uart.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_sd.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_ll_sdmmc.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_dma.c

SRCS += Src/system_stm32f4xx.c
SRCS += Src/stm32f4xx_it.c
SRCS += Src/stm32f4xx_newlib_stubs.c
SRCS += Src/stm32f4xx_hal_msp.c

SRCS += $(FATFS_PATH)/ff.c
SRCS += $(FATFS_PATH)/diskio.c
SRCS += $(FATFS_PATH)/drivers/fatfs_sd_sdio.c
SRCS += $(FATFS_PATH)/option/syscall.c
SRCS += $(FATFS_PATH)/option/ccsbcs.c

SRCS += Src/main.c

OBJS  = ccsbcs.o main.o stm32f4xx_hal_dma.o stm32f4xx_hal_rcc.o stm32f4xx_ll_sdmmc.o
OBJS += diskio.o startup_stm32f407xx.o stm32f4xx_hal_gpio.o stm32f4xx_hal_sd.o stm32f4xx_newlib_stubs.o
OBJS += fatfs_sd_sdio.o stm32f4_discovery.o stm32f4xx_hal_msp.o stm32f4xx_hal_uart.o syscall.o
OBJS += ff.o stm32f4xx_hal_cortex.o stm32f4xx_hal.o stm32f4xx_it.o system_stm32f4xx.o
OBJS += external_libs/molup/static_molup.o

#define build targets
$(TARGET).elf:
	$(CC) $(CFLAGS) $(SRCS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
	@$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	@$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

all: $(TARGET).elf
	@echo -e "\033[0;93mBuild Successful\033[0;0m"

clean:
	rm -rf *.o *.elf *.hex *.bin *.map 2>/dev/null

burn:$(TARGET).bin
	$(STLINK)/st-flash write $(TARGET).bin 0x8000000

debug:$(TARGET).bin
	$(STLINK)/st-util -p 5000 & echo $! > /tmp/stmgdbserver.pid
	$(DEBUGGER) $(TARGET).elf -ex "target remote :5000" -ex "b main" -ex "set print pretty on" -ex "continue"

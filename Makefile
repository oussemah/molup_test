#define TARGET Name
TARGET=template

#define TOOLCHAIN
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

DEBUGGER=arm-none-eabi-gdb
STLINK=/opt/STM/stlink


#define Toolchain and Library paths
LIB_PATH   ?=/usr/lib/arm-none-eabi/newlib/armv7e-m

HAL_PATH   ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Drivers/STM32F4xx_HAL_Driver
CMSIS_PATH ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Drivers/CMSIS
BSP_PATH   ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Drivers/BSP

FATFS_PATH ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Middlewares/Third_Party/fatfs_custom

#define INCLUDES
BASE_INCLUDES= -I$(LIB_PATH)/include-fixed

HAL_INCLUDES= -I$(HAL_PATH)/Inc -I$(CMSIS_PATH)/Include -I$(CMSIS_PATH)/Device/ST/STM32F4xx/Include

CUSTOM_INCLUDES= -IInc -I$(BSP_PATH)/STM32F4-Discovery -I$(FATFS_PATH) -I$(FATFS_PATH)/drivers

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

LDFLAGS = -L /usr/lib/gcc/arm-none-eabi/4.9.3/armv7e-m -lgcc -L $(LIB_PATH) -lc -TSTM32F407VGTx_FLASH.ld -Map $(TARGET).map -nostartfiles

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

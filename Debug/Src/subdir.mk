################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/GUIConf.c \
../Src/LCDConf.c \
../Src/adc.c \
../Src/crc.c \
../Src/fsmc.c \
../Src/ft6206.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/ili_9341.c \
../Src/main.c \
../Src/rtc.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/tim.c 

OBJS += \
./Src/GUIConf.o \
./Src/LCDConf.o \
./Src/adc.o \
./Src/crc.o \
./Src/fsmc.o \
./Src/ft6206.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/ili_9341.o \
./Src/main.o \
./Src/rtc.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/tim.o 

C_DEPS += \
./Src/GUIConf.d \
./Src/LCDConf.d \
./Src/adc.d \
./Src/crc.d \
./Src/fsmc.d \
./Src/ft6206.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/ili_9341.d \
./Src/main.d \
./Src/rtc.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -pedantic -Wall -Wmissing-declarations  -g3 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSI_VALUE=168000000 -D__FPU_USED -D__FPU_PRESENT -DARM_MATH_CM4 -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Include" -I"C:/stm32ws/vva_tstat/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/stm32ws/vva_tstat/Inc" -I"C:/stm32ws/vva_tstat/STemWin/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



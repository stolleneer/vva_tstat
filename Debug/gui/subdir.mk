################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gui/HomeScreenDLG.c \
../gui/SplashScreenDLG.c 

OBJS += \
./gui/HomeScreenDLG.o \
./gui/SplashScreenDLG.o 

C_DEPS += \
./gui/HomeScreenDLG.d \
./gui/SplashScreenDLG.d 


# Each subdirectory must supply rules for building sources it contributes
gui/%.o: ../gui/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -pedantic -Wall -Wmissing-declarations  -g3 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSI_VALUE=168000000 -D__FPU_USED -D__FPU_PRESENT -DARM_MATH_CM4 -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Include" -I"C:/stm32ws/vva_tstat/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/stm32ws/vva_tstat/Inc" -I"C:/stm32ws/vva_tstat/STemWin/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



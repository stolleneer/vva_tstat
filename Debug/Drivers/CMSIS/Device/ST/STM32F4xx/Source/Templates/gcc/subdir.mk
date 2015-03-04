################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.asm 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.o 

ASM_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/%.o: ../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -pedantic -Wall -Wmissing-declarations  -g3 -x assembler-with-cpp -DUSE_HAL_DRIVER -DSTM32F407xx -DHSI_VALUE=168000000 -D__FPU_USED -D__FPU_PRESENT -DARM_MATH_CM4 -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/stm32ws/vva_tstat/Drivers/CMSIS/Include" -I"C:/stm32ws/vva_tstat/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/stm32ws/vva_tstat/Inc" -I"C:/stm32ws/vva_tstat/STemWin/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



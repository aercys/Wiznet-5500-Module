################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ioLibrary_Driver/Ethernet/W5100S/w5100s.c 

OBJS += \
./ioLibrary_Driver/Ethernet/W5100S/w5100s.o 

C_DEPS += \
./ioLibrary_Driver/Ethernet/W5100S/w5100s.d 


# Each subdirectory must supply rules for building sources it contributes
ioLibrary_Driver/Ethernet/W5100S/%.o: ../ioLibrary_Driver/Ethernet/W5100S/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/Users/atakane/Documents/electronics/W5500TCP_Example/ioLibrary_Driver/Ethernet" -I"/Users/atakane/Documents/electronics/W5500TCP_Example/ioLibrary_Driver/Ethernet/W5500" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ioLibrary/Ethernet/socket.c \
../ioLibrary/Ethernet/wizchip_conf.c 

OBJS += \
./ioLibrary/Ethernet/socket.o \
./ioLibrary/Ethernet/wizchip_conf.o 

C_DEPS += \
./ioLibrary/Ethernet/socket.d \
./ioLibrary/Ethernet/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
ioLibrary/Ethernet/%.o: ../ioLibrary/Ethernet/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/Users/atakane/Documents/electronics/W5500TCP_Example/ioLibrary/Ethernet" -I"/Users/atakane/Documents/electronics/W5500TCP_Example/ioLibrary/Ethernet/W5500" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



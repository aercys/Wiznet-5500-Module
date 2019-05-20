################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/stm32f1-stdperiph/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/subdir.mk
-include ioLibrary_Driver/Internet/httpServer/subdir.mk
-include ioLibrary_Driver/Internet/TFTP/subdir.mk
-include ioLibrary_Driver/Internet/SNTP/subdir.mk
-include ioLibrary_Driver/Internet/SNMP/subdir.mk
-include ioLibrary_Driver/Internet/MQTT/MQTTPacket/src/subdir.mk
-include ioLibrary_Driver/Internet/MQTT/subdir.mk
-include ioLibrary_Driver/Internet/FTPServer/subdir.mk
-include ioLibrary_Driver/Internet/FTPClient/subdir.mk
-include ioLibrary_Driver/Internet/DNS/subdir.mk
-include ioLibrary_Driver/Internet/DHCP/subdir.mk
-include ioLibrary_Driver/Ethernet/W5500/subdir.mk
-include ioLibrary_Driver/Ethernet/W5300/subdir.mk
-include ioLibrary_Driver/Ethernet/W5200/subdir.mk
-include ioLibrary_Driver/Ethernet/W5100S/subdir.mk
-include ioLibrary_Driver/Ethernet/W5100/subdir.mk
-include ioLibrary_Driver/Ethernet/subdir.mk
-include ioLibrary_Driver/Application/loopback/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
W5500TCP_Example.hex \

SECONDARY_SIZE += \
W5500TCP_Example.siz \


# All Target
all: W5500TCP_Example.elf secondary-outputs

# Tool invocations
W5500TCP_Example.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"W5500TCP_Example.map" --specs=nano.specs -o "W5500TCP_Example.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

W5500TCP_Example.hex: W5500TCP_Example.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "W5500TCP_Example.elf"  "W5500TCP_Example.hex"
	@echo 'Finished building: $@'
	@echo ' '

W5500TCP_Example.siz: W5500TCP_Example.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "W5500TCP_Example.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) W5500TCP_Example.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets

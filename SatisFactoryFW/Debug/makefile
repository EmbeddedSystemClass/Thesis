################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include uwbCore/platform/subdir.mk
-include uwbCore/decadriver/subdir.mk
-include uwbCore/application/subdir.mk
-include system/src/stm32f4xx/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include system/src/FreeRTOS/subdir.mk
-include mpuCore/mllite/subdir.mk
-include mpuCore/eMPL-hal/subdir.mk
-include mpuCore/driver/stm32L/subdir.mk
-include mpuCore/driver/eMPL/subdir.mk
-include User/src/subdir.mk
-include Peripheral/src/subdir.mk
-include DeviceSupport/src/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
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
SatisFactoryFW.hex \

SECONDARY_SIZE += \
SatisFactoryFW.siz \


# All Target
all: SatisFactoryFW.elf secondary-outputs

# Tool invocations
SatisFactoryFW.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T "C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\ldscripts\mem.ld" -T "C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\ldscripts\libs.ld" -T "C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\ldscripts\sections.ld" -nostartfiles -Xlinker --gc-sections -L"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\mpl" -Wl,-Map,"SatisFactoryFW.map" -o "SatisFactoryFW.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

SatisFactoryFW.hex: SatisFactoryFW.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "SatisFactoryFW.elf"  "SatisFactoryFW.hex"
	@echo 'Finished building: $@'
	@echo ' '

SatisFactoryFW.siz: SatisFactoryFW.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "SatisFactoryFW.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) SatisFactoryFW.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

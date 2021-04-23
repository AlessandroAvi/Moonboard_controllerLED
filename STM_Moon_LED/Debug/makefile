################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM_Moon_LED.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM_Moon_LED.list \

OBJCOPY_BIN += \
STM_Moon_LED.bin \


# All Target
all: STM_Moon_LED.elf secondary-outputs

# Tool invocations
STM_Moon_LED.elf: $(OBJS) $(USER_OBJS) C:\Users\Matteo\Documents\UNI\Meccatronica\2y\GIT\ projects\Mechatronics\ -\ \ Shared\ Projects\ Folder\Moonboard_LED_DIY\STM_Moon_LED\STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "STM_Moon_LED.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Matteo\Documents\UNI\Meccatronica\2y\GIT projects\Mechatronics -  Shared Projects Folder\Moonboard_LED_DIY\STM_Moon_LED\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM_Moon_LED.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM_Moon_LED.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM_Moon_LED.list"
	@echo 'Finished building: $@'
	@echo ' '

STM_Moon_LED.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM_Moon_LED.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

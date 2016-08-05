################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uwbCore/application/instance.c \
../uwbCore/application/instance_calib.c \
../uwbCore/application/instance_common.c \
../uwbCore/application/uwbMain.c 

OBJS += \
./uwbCore/application/instance.o \
./uwbCore/application/instance_calib.o \
./uwbCore/application/instance_common.o \
./uwbCore/application/uwbMain.o 

C_DEPS += \
./uwbCore/application/instance.d \
./uwbCore/application/instance_calib.d \
./uwbCore/application/instance_common.d \
./uwbCore/application/uwbMain.d 


# Each subdirectory must supply rules for building sources it contributes
uwbCore/application/%.o: ../uwbCore/application/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DTRACE -DMPU9250 -DMPL_LOG_NDEBUG=1 -DEMPL -DUSE_DMP -DSTM32F411xE -DEMPL_TARGET_STM32F4 -I"D:\Mateo\Workspace\SatisFactoryFW\system\include" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\stm32L" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\eMPL-hal" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\eMPL" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\include" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\cmsis" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\stm32f4xx" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\FreeRTOS" -I"D:\Mateo\Workspace\SatisFactoryFW\Peripheral\inc" -I"D:\Mateo\Workspace\SatisFactoryFW\DeviceSupport\inc" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\mpl" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\mllite" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\platform" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\decadriver" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\application" -I"D:\Mateo\Workspace\SatisFactoryFW\User\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



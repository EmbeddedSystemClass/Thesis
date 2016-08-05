################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uwbCore/platform/deca_mutex.c \
../uwbCore/platform/deca_port.c \
../uwbCore/platform/deca_spi.c \
../uwbCore/platform/dma_spi.c 

OBJS += \
./uwbCore/platform/deca_mutex.o \
./uwbCore/platform/deca_port.o \
./uwbCore/platform/deca_spi.o \
./uwbCore/platform/dma_spi.o 

C_DEPS += \
./uwbCore/platform/deca_mutex.d \
./uwbCore/platform/deca_port.d \
./uwbCore/platform/deca_spi.d \
./uwbCore/platform/dma_spi.d 


# Each subdirectory must supply rules for building sources it contributes
uwbCore/platform/%.o: ../uwbCore/platform/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DTRACE -DMPU9250 -DMPL_LOG_NDEBUG=1 -DEMPL -DUSE_DMP -DSTM32F411xE -DEMPL_TARGET_STM32F4 -I"D:\Mateo\Workspace\SatisFactoryFW\system\include" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\stm32L" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\eMPL-hal" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\eMPL" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\driver\include" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\cmsis" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\stm32f4xx" -I"D:\Mateo\Workspace\SatisFactoryFW\system\include\FreeRTOS" -I"D:\Mateo\Workspace\SatisFactoryFW\Peripheral\inc" -I"D:\Mateo\Workspace\SatisFactoryFW\DeviceSupport\inc" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\mpl" -I"D:\Mateo\Workspace\SatisFactoryFW\mpuCore\mllite" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\platform" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\decadriver" -I"D:\Mateo\Workspace\SatisFactoryFW\uwbCore\application" -I"D:\Mateo\Workspace\SatisFactoryFW\User\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/src/_write.c \
../User/src/freertos.c \
../User/src/main.c 

OBJS += \
./User/src/_write.o \
./User/src/freertos.o \
./User/src/main.o 

C_DEPS += \
./User/src/_write.d \
./User/src/freertos.d \
./User/src/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/src/%.o: ../User/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DTRACE -DMPU9250 -DMPL_LOG_NDEBUG=1 -DEMPL -DUSE_DMP -DSTM32F411xE -DEMPL_TARGET_STM32F4 -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\system\include" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\driver\stm32L" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\eMPL-hal" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\driver\eMPL" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\driver\include" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\system\include\cmsis" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\system\include\stm32f4xx" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\system\include\FreeRTOS" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\Peripheral\inc" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\DeviceSupport\inc" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\mpl" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\mpuCore\mllite" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\uwbCore\platform" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\uwbCore\decadriver" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\uwbCore\application" -I"C:\Users\damag\Documents\MATEO\POLITO\Thesis\gitrepository\SatisFactoryFW\User\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



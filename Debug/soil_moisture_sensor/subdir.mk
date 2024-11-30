################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../soil_moisture_sensor/soil_moisture_sensor.c 

OBJS += \
./soil_moisture_sensor/soil_moisture_sensor.o 

C_DEPS += \
./soil_moisture_sensor/soil_moisture_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
soil_moisture_sensor/%.o soil_moisture_sensor/%.su soil_moisture_sensor/%.cyclo: ../soil_moisture_sensor/%.c soil_moisture_sensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"C:/MyFiles/Study/7th_sem/VRS/Workspace/ground-humidity-sensor/hw038" -I"C:/MyFiles/Study/7th_sem/VRS/Workspace/ground-humidity-sensor/soil_moisture_sensor" -I"C:/MyFiles/Study/7th_sem/VRS/Workspace/ground-humidity-sensor/pump" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-soil_moisture_sensor

clean-soil_moisture_sensor:
	-$(RM) ./soil_moisture_sensor/soil_moisture_sensor.cyclo ./soil_moisture_sensor/soil_moisture_sensor.d ./soil_moisture_sensor/soil_moisture_sensor.o ./soil_moisture_sensor/soil_moisture_sensor.su

.PHONY: clean-soil_moisture_sensor


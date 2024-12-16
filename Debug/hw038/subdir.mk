################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hw038/hw-038.c 

OBJS += \
./hw038/hw-038.o 

C_DEPS += \
./hw038/hw-038.d 


# Each subdirectory must supply rules for building sources it contributes
hw038/%.o hw038/%.su hw038/%.cyclo: ../hw038/%.c hw038/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/hw038" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/soil_moisture_sensor" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/pump" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/lcd" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-hw038

clean-hw038:
	-$(RM) ./hw038/hw-038.cyclo ./hw038/hw-038.d ./hw038/hw-038.o ./hw038/hw-038.su

.PHONY: clean-hw038


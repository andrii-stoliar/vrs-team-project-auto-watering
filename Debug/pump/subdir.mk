################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../pump/pump.c 

OBJS += \
./pump/pump.o 

C_DEPS += \
./pump/pump.d 


# Each subdirectory must supply rules for building sources it contributes
pump/%.o pump/%.su pump/%.cyclo: ../pump/%.c pump/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/hw038" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/soil_moisture_sensor" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/pump" -I"/home/godmaze/STM32CubeIDE/workspace_1.16.1/projekt/vrs-team-project-auto-watering/lcd" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-pump

clean-pump:
	-$(RM) ./pump/pump.cyclo ./pump/pump.d ./pump/pump.o ./pump/pump.su

.PHONY: clean-pump


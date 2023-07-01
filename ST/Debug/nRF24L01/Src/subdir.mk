################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nRF24L01/Src/nRF24L01.c 

OBJS += \
./nRF24L01/Src/nRF24L01.o 

C_DEPS += \
./nRF24L01/Src/nRF24L01.d 


# Each subdirectory must supply rules for building sources it contributes
nRF24L01/Src/%.o nRF24L01/Src/%.su nRF24L01/Src/%.cyclo: ../nRF24L01/Src/%.c nRF24L01/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/MONSTER/STM32CubeIDE/workspace_1.11.0/RF/ST7735/Inc" -I"C:/Users/MONSTER/STM32CubeIDE/workspace_1.11.0/RF/nRF24L01/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-nRF24L01-2f-Src

clean-nRF24L01-2f-Src:
	-$(RM) ./nRF24L01/Src/nRF24L01.cyclo ./nRF24L01/Src/nRF24L01.d ./nRF24L01/Src/nRF24L01.o ./nRF24L01/Src/nRF24L01.su

.PHONY: clean-nRF24L01-2f-Src


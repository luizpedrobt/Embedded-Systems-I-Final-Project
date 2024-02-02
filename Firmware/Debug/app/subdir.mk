################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app.c \
../app/hard.c \
../app/max30100.c \
../app/ssd1306.c \
../app/ssd1306_font.c 

OBJS += \
./app/app.o \
./app/hard.o \
./app/max30100.o \
./app/ssd1306.o \
./app/ssd1306_font.o 

C_DEPS += \
./app/app.d \
./app/hard.d \
./app/max30100.d \
./app/ssd1306.d \
./app/ssd1306_font.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L072xx -c -I../app -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/app.cyclo ./app/app.d ./app/app.o ./app/app.su ./app/hard.cyclo ./app/hard.d ./app/hard.o ./app/hard.su ./app/max30100.cyclo ./app/max30100.d ./app/max30100.o ./app/max30100.su ./app/ssd1306.cyclo ./app/ssd1306.d ./app/ssd1306.o ./app/ssd1306.su ./app/ssd1306_font.cyclo ./app/ssd1306_font.d ./app/ssd1306_font.o ./app/ssd1306_font.su

.PHONY: clean-app


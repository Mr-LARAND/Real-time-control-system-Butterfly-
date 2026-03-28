################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/abstraction_layer.c \
../Core/Src/epos2.c \
../Core/Src/heartbeat.c \
../Core/Src/main.c \
../Core/Src/nmt.c \
../Core/Src/pdo.c \
../Core/Src/sdo.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

OBJS += \
./Core/Src/abstraction_layer.o \
./Core/Src/epos2.o \
./Core/Src/heartbeat.o \
./Core/Src/main.o \
./Core/Src/nmt.o \
./Core/Src/pdo.o \
./Core/Src/sdo.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o 

C_DEPS += \
./Core/Src/abstraction_layer.d \
./Core/Src/epos2.d \
./Core/Src/heartbeat.d \
./Core/Src/main.d \
./Core/Src/nmt.d \
./Core/Src/pdo.d \
./Core/Src/sdo.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I/home/larand/STM_Projects/Batterfly_test/CANopenNode/301 -I/home/larand/STM_Projects/Batterfly_test/CANopenNode/305 -I/home/larand/STM_Projects/Batterfly_test/CANopenNode/storage -I/home/larand/STM_Projects/Batterfly_test/CANopenNode -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/abstraction_layer.cyclo ./Core/Src/abstraction_layer.d ./Core/Src/abstraction_layer.o ./Core/Src/abstraction_layer.su ./Core/Src/epos2.cyclo ./Core/Src/epos2.d ./Core/Src/epos2.o ./Core/Src/epos2.su ./Core/Src/heartbeat.cyclo ./Core/Src/heartbeat.d ./Core/Src/heartbeat.o ./Core/Src/heartbeat.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/nmt.cyclo ./Core/Src/nmt.d ./Core/Src/nmt.o ./Core/Src/nmt.su ./Core/Src/pdo.cyclo ./Core/Src/pdo.d ./Core/Src/pdo.o ./Core/Src/pdo.su ./Core/Src/sdo.cyclo ./Core/Src/sdo.d ./Core/Src/sdo.o ./Core/Src/sdo.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su

.PHONY: clean-Core-2f-Src


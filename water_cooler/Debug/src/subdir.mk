################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client.c \
../src/common.c \
../src/packet.c \
../src/server.c \
../src/util.c \
../src/water_cooler.c 

OBJS += \
./src/client.o \
./src/common.o \
./src/packet.o \
./src/server.o \
./src/util.o \
./src/water_cooler.o 

C_DEPS += \
./src/client.d \
./src/common.d \
./src/packet.d \
./src/server.d \
./src/util.d \
./src/water_cooler.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/sayaka/workspace/mynet" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/ImageInfoLoader.cpp 

OBJS += \
./libs/ImageInfoLoader.o 

CPP_DEPS += \
./libs/ImageInfoLoader.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -I/usr/include/cppconn -I/usr/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



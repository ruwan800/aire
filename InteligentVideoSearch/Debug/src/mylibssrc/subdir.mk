################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mylibssrc/BayesClassifier.cpp \
../src/mylibssrc/DataPreProcessor.cpp \
../src/mylibssrc/DataStore.cpp \
../src/mylibssrc/ImageInfoLoader.cpp \
../src/mylibssrc/Util.cpp 

OBJS += \
./src/mylibssrc/BayesClassifier.o \
./src/mylibssrc/DataPreProcessor.o \
./src/mylibssrc/DataStore.o \
./src/mylibssrc/ImageInfoLoader.o \
./src/mylibssrc/Util.o 

CPP_DEPS += \
./src/mylibssrc/BayesClassifier.d \
./src/mylibssrc/DataPreProcessor.d \
./src/mylibssrc/DataStore.d \
./src/mylibssrc/ImageInfoLoader.d \
./src/mylibssrc/Util.d 


# Each subdirectory must supply rules for building sources it contributes
src/mylibssrc/%.o: ../src/mylibssrc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -I/usr/include/cppconn -I/usr/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags aire` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



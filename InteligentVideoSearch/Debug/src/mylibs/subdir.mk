################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mylibs/BayesClassifier.cpp \
../src/mylibs/DataStore.cpp \
../src/mylibs/ImageInfoLoader.cpp \
../src/mylibs/Util.cpp 

OBJS += \
./src/mylibs/BayesClassifier.o \
./src/mylibs/DataStore.o \
./src/mylibs/ImageInfoLoader.o \
./src/mylibs/Util.o 

CPP_DEPS += \
./src/mylibs/BayesClassifier.d \
./src/mylibs/DataStore.d \
./src/mylibs/ImageInfoLoader.d \
./src/mylibs/Util.d 


# Each subdirectory must supply rules for building sources it contributes
src/mylibs/%.o: ../src/mylibs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -I/usr/include/cppconn -I/usr/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



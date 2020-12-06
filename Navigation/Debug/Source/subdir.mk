################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/A_Star.cpp \
../Source/Graph.cpp \
../Source/Map.cpp \
../Source/Tree.cpp \
../Source/main.cpp 

OBJS += \
./Source/A_Star.o \
./Source/Graph.o \
./Source/Map.o \
./Source/Tree.o \
./Source/main.o 

CPP_DEPS += \
./Source/A_Star.d \
./Source/Graph.d \
./Source/Map.d \
./Source/Tree.d \
./Source/main.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



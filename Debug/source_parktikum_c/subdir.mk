################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../source_parktikum_c/file_reader.o 

C_SRCS += \
../source_parktikum_c/dictionary.c \
../source_parktikum_c/file_reader.c \
../source_parktikum_c/file_reader_fgets.c \
../source_parktikum_c/list.c \
../source_parktikum_c/parser.c 

OBJS += \
./source_parktikum_c/dictionary.o \
./source_parktikum_c/file_reader.o \
./source_parktikum_c/file_reader_fgets.o \
./source_parktikum_c/list.o \
./source_parktikum_c/parser.o 

C_DEPS += \
./source_parktikum_c/dictionary.d \
./source_parktikum_c/file_reader.d \
./source_parktikum_c/file_reader_fgets.d \
./source_parktikum_c/list.d \
./source_parktikum_c/parser.d 


# Each subdirectory must supply rules for building sources it contributes
source_parktikum_c/%.o: ../source_parktikum_c/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



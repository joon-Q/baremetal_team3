################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32h743vitx.s 

S_DEPS += \
./Startup/startup_stm32h743vitx.d 

OBJS += \
./Startup/startup_stm32h743vitx.o 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/kissfft" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/gemmlowp" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/ruy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/flatbuffers/include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/cmsis_nn/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM/third_party/cmsis_nn" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-05_TFLM_MNIST_CNN_QAT/TFLM" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32h743vitx.d ./Startup/startup_stm32h743vitx.o

.PHONY: clean-Startup


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/kernels/kernel_util.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/kernels/kernel_util.d 

OBJS += \
./TFLM/tensorflow/lite/kernels/kernel_util.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/kernels/%.o TFLM/tensorflow/lite/kernels/%.su TFLM/tensorflow/lite/kernels/%.cyclo: ../TFLM/tensorflow/lite/kernels/%.cc TFLM/tensorflow/lite/kernels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/ST7735" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/Camera" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/kissfft" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/gemmlowp" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/ruy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/flatbuffers/include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-kernels

clean-TFLM-2f-tensorflow-2f-lite-2f-kernels:
	-$(RM) ./TFLM/tensorflow/lite/kernels/kernel_util.cyclo ./TFLM/tensorflow/lite/kernels/kernel_util.d ./TFLM/tensorflow/lite/kernels/kernel_util.o ./TFLM/tensorflow/lite/kernels/kernel_util.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-kernels


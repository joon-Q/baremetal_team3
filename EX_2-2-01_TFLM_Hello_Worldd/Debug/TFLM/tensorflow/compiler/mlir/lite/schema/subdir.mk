################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.cc 

CC_DEPS += \
./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.d 

OBJS += \
./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/compiler/mlir/lite/schema/%.o TFLM/tensorflow/compiler/mlir/lite/schema/%.su TFLM/tensorflow/compiler/mlir/lite/schema/%.cyclo: ../TFLM/tensorflow/compiler/mlir/lite/schema/%.cc TFLM/tensorflow/compiler/mlir/lite/schema/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/ST7735" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/Camera" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/kissfft" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/gemmlowp" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/ruy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/flatbuffers/include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-compiler-2f-mlir-2f-lite-2f-schema

clean-TFLM-2f-tensorflow-2f-compiler-2f-mlir-2f-lite-2f-schema:
	-$(RM) ./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.cyclo ./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.d ./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.o ./TFLM/tensorflow/compiler/mlir/lite/schema/schema_utils.su

.PHONY: clean-TFLM-2f-tensorflow-2f-compiler-2f-mlir-2f-lite-2f-schema


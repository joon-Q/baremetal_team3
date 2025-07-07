################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/micro/models/person_detect_model_data.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/micro/models/person_detect_model_data.d 

OBJS += \
./TFLM/tensorflow/lite/micro/models/person_detect_model_data.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/micro/models/%.o TFLM/tensorflow/lite/micro/models/%.su TFLM/tensorflow/lite/micro/models/%.cyclo: ../TFLM/tensorflow/lite/micro/models/%.cc TFLM/tensorflow/lite/micro/models/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/BSP/ST7735" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/BSP/Camera" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/CMSIS/Include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/kissfft" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/gemmlowp" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/ruy" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/flatbuffers/include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/cmsis_nn/Include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/cmsis_nn" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-models

clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-models:
	-$(RM) ./TFLM/tensorflow/lite/micro/models/person_detect_model_data.cyclo ./TFLM/tensorflow/lite/micro/models/person_detect_model_data.d ./TFLM/tensorflow/lite/micro/models/person_detect_model_data.o ./TFLM/tensorflow/lite/micro/models/person_detect_model_data.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-models


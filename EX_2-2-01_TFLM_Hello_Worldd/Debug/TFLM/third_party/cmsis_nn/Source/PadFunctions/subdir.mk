################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.c 

C_DEPS += \
./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.d 

OBJS += \
./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/third_party/cmsis_nn/Source/PadFunctions/%.o TFLM/third_party/cmsis_nn/Source/PadFunctions/%.su TFLM/third_party/cmsis_nn/Source/PadFunctions/%.cyclo: ../TFLM/third_party/cmsis_nn/Source/PadFunctions/%.c TFLM/third_party/cmsis_nn/Source/PadFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DTFT96 -D__USE_CMSIS -DCMSIS_NN -DCMSIS_OPTIMIZATION_LEVEL -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/CMSIS/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/Camera" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/Drivers/BSP/ST7735" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/kissfft" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/gemmlowp" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/ruy" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/flatbuffers/include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn/Include" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM/third_party/cmsis_nn" -I"C:/Baremetal_ODAI_STM32H7/EX_2-2-01_TFLM_Hello_Worldd/TFLM" -O3 -ffunction-sections -fdata-sections -lnosys -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PadFunctions

clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PadFunctions:
	-$(RM) ./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.d ./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.o ./TFLM/third_party/cmsis_nn/Source/PadFunctions/arm_pad_s8.su

.PHONY: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PadFunctions


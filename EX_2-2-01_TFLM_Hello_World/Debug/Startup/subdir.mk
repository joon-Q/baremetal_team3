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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/kissfft" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/gemmlowp" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/ruy" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/flatbuffers/include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/cmsis_nn/Include" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM/third_party/cmsis_nn" -I"C:/joon/baremetal_team3-flower102/baremetal_team3-flower102/EX_2-2-01_TFLM_Hello_World/TFLM" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32h743vitx.d ./Startup/startup_stm32h743vitx.o

.PHONY: clean-Startup


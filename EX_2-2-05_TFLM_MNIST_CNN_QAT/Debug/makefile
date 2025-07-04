################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include models/subdir.mk
-include TFLM/third_party/kissfft/tools/subdir.mk
-include TFLM/third_party/kissfft/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/TransposeFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/SoftmaxFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/SVDFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/ReshapeFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/PoolingFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/PadFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/LSTMFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/FullyConnectedFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/ConvolutionFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/ConcatenationFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/BasicMathFunctions/subdir.mk
-include TFLM/third_party/cmsis_nn/Source/ActivationFunctions/subdir.mk
-include TFLM/tensorflow/lite/micro/tflite_bridge/subdir.mk
-include TFLM/tensorflow/lite/micro/models/subdir.mk
-include TFLM/tensorflow/lite/micro/memory_planner/subdir.mk
-include TFLM/tensorflow/lite/micro/kernels/cmsis_nn/subdir.mk
-include TFLM/tensorflow/lite/micro/kernels/subdir.mk
-include TFLM/tensorflow/lite/micro/cortex_m_generic/subdir.mk
-include TFLM/tensorflow/lite/micro/arena_allocator/subdir.mk
-include TFLM/tensorflow/lite/micro/subdir.mk
-include TFLM/tensorflow/lite/kernels/internal/reference/subdir.mk
-include TFLM/tensorflow/lite/kernels/internal/subdir.mk
-include TFLM/tensorflow/lite/kernels/subdir.mk
-include TFLM/tensorflow/lite/core/c/subdir.mk
-include TFLM/tensorflow/lite/core/api/subdir.mk
-include TFLM/tensorflow/compiler/mlir/lite/schema/subdir.mk
-include TFLM/tensorflow/compiler/mlir/lite/core/api/subdir.mk
-include TFLM/signal/src/kiss_fft_wrappers/subdir.mk
-include TFLM/signal/src/subdir.mk
-include TFLM/signal/micro/kernels/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/ST7735/subdir.mk
-include Drivers/BSP/Camera/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := EX_2-2-05_TFLM_MNIST_CNN_QAT
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EX_2-2-05_TFLM_MNIST_CNN_QAT.elf \

MAP_FILES += \
EX_2-2-05_TFLM_MNIST_CNN_QAT.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EX_2-2-05_TFLM_MNIST_CNN_QAT.list \


# All Target
all: main-build

# Main-build Target
main-build: EX_2-2-05_TFLM_MNIST_CNN_QAT.elf secondary-outputs

# Tool invocations
EX_2-2-05_TFLM_MNIST_CNN_QAT.elf EX_2-2-05_TFLM_MNIST_CNN_QAT.map: $(OBJS) $(USER_OBJS) C:\Baremetal_ODAI_STM32H7\EX_2-2-05_TFLM_MNIST_CNN_QAT\STM32H743VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "EX_2-2-05_TFLM_MNIST_CNN_QAT.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Baremetal_ODAI_STM32H7\EX_2-2-05_TFLM_MNIST_CNN_QAT\STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EX_2-2-05_TFLM_MNIST_CNN_QAT.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EX_2-2-05_TFLM_MNIST_CNN_QAT.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EX_2-2-05_TFLM_MNIST_CNN_QAT.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) EX_2-2-05_TFLM_MNIST_CNN_QAT.elf EX_2-2-05_TFLM_MNIST_CNN_QAT.list EX_2-2-05_TFLM_MNIST_CNN_QAT.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets

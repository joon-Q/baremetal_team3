/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "uart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "camera.h"
#include "lcd.h"

#include <stdio.h>

#include "tensorflow/lite/core/c/common.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_profiler.h"

#include "models/mnist_qat_model.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

#ifdef TFT96
// QQVGA
#define FrameWidth 160
#define FrameHeight 120
#elif TFT18
// QQVGA2
#define FrameWidth 128
#define FrameHeight 160
#endif
// picture buffer
uint16_t pic[FrameHeight][FrameWidth];
volatile uint32_t DCMI_FrameIsReady;
uint32_t Camera_FPS=0;

constexpr int tensor_arena_size = 50 * 1024;
static uint8_t tensor_arena[tensor_arena_size];

int Get_Top_Prediction(const float* predictions, int num_categories);
void Get_Data(void* target);
void Get_Capture(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	/* Disables the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes for the QSPI 256MB without instruction access */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress      = QSPI_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes for the QSPI 8MB (QSPI Flash Size) to Cacheable WT */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
	MPU_InitStruct.BaseAddress      = QSPI_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Setup AXI SRAM in Cacheable WB */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

void LED_Blink(uint32_t Hdelay, uint32_t Ldelay)
{
	HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_SET);
	HAL_Delay(Hdelay - 1);
	HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_RESET);
	HAL_Delay(Ldelay - 1);
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

#ifdef W25Qxx
	SCB->VTOR = QSPI_BASE;
#endif
	MPU_Config();
	CPU_CACHE_Enable();

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_DCMI_Init();
	MX_I2C1_Init();
	MX_SPI4_Init();
	MX_TIM1_Init();
	UART_Init();
	/* USER CODE BEGIN 2 */
	uint8_t text[80];

	LCD_Test();

	sprintf((char *)&text, "Camera Not Found");
	LCD_ShowString(0, 58, ST7735Ctx.Width, 16, 16, text);

	//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	//	HAL_Delay(10);
#ifdef TFT96
	Camera_Init_Device(&hi2c1, FRAMESIZE_QQVGA);
#elif TFT18
	Camera_Init_Device(&hi2c1, FRAMESIZE_QQVGA2);
#endif

	//clean Ypos 58
	ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 58, ST7735Ctx.Width, 16, BLACK);

	UART_Send_String((char*)"MNIST Classification (CNN, Quantization-Aware-Training) \n\r");

	const tflite::Model* model = ::tflite::GetModel(mnist_qat_model);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		for(;;);
	}

	UART_Send_String((char*)"TFLITE_SCHEMA_VERSION OK!\n\r");

	tflite::MicroMutableOpResolver<10> micro_op_resolver;

	micro_op_resolver.AddConv2D();
	micro_op_resolver.AddDequantize();
	micro_op_resolver.AddFullyConnected();
	micro_op_resolver.AddStridedSlice();
	micro_op_resolver.AddShape();
	micro_op_resolver.AddMaxPool2D();
	micro_op_resolver.AddQuantize();
	micro_op_resolver.AddPack();
	micro_op_resolver.AddReshape();
	micro_op_resolver.AddSoftmax();

	tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size);
	if(interpreter.AllocateTensors() != kTfLiteOk){
		sprintf((char *)&text, "TFLM Allocate Tensor Failed!!");
		UART_Send_String((char*)text);
		while(1);
	}

	sprintf((char *)&text, "Tensor Arena used : %d\n\r", interpreter.arena_used_bytes());
	UART_Send_String((char*)text);
	TfLiteTensor* input = interpreter.input(0);
	sprintf((char *)&text, "Input_Shape : (%d,%d,%d)\n\r",input->dims->data[0], input->dims->data[1], input->dims->data[2]) ;
	UART_Send_String((char*)text);

	while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
	{
		sprintf((char *)&text, "Camera id:0x%x   ", hcamera.device_id);
		LCD_ShowString(0, 58, ST7735Ctx.Width, 16, 12, text);

		LED_Blink(5, 500);

		sprintf((char *)&text, "LongPress K1 to Run");
		LCD_ShowString(0, 58, ST7735Ctx.Width, 16, 12, text);

		LED_Blink(5, 500);
	}

	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)&pic, FrameWidth * FrameHeight * 2 / 4);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (DCMI_FrameIsReady)
		{
			HAL_DCMI_Stop(&hdcmi);
			DCMI_FrameIsReady = 0;

			int time_start_lcd = HAL_GetTick();
			ST7735_FillRGBRect(&st7735_pObj,0,0,(uint8_t *)&pic[20][0], ST7735Ctx.Width, 80);
			int time_end_lcd = HAL_GetTick();

			// Draw Input Data Guide Line
			ST7735_DrawVLine(&st7735_pObj, 160/2-28, 80/2-28, 56, 0xf800);
			ST7735_DrawVLine(&st7735_pObj, 160/2+28, 80/2-28, 56, 0xf800);
			ST7735_DrawHLine(&st7735_pObj, 160/2-28, 80/2-28, 56, 0xf800);
			ST7735_DrawHLine(&st7735_pObj, 160/2-28, 80/2+28, 56, 0xf800);

			int time_start_getdata = HAL_GetTick();
			Get_Data(input->data.f);
			int time_end_getdata = HAL_GetTick();

			int time_start_invoke = HAL_GetTick();
			TfLiteStatus invoke_status = interpreter.Invoke();
			if (invoke_status != kTfLiteOk) {
				UART_Send_String((char*)"Invoke failed.\n\r");
				break;
			}
			int time_end_invoke = HAL_GetTick();

			TfLiteTensor* output = interpreter.output(0);
			int top_ind = Get_Top_Prediction(output->data.f, 10);

			sprintf((char *)&text, "%d %.1f%%\n\r", top_ind, output->data.f[top_ind] * 100.0);
			UART_Send_String((char*)text);

			sprintf((char *)&text, "%d\n\r", top_ind);
			LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 12, text);

			sprintf((char *)&text, "Cam to LCD : %dms\n\r",time_end_lcd - time_start_lcd);
			UART_Send_String((char*)text);

			sprintf((char *)&text, "Get_Data : %dms\n\r",time_end_getdata - time_start_getdata);
			UART_Send_String((char*)text);

			sprintf((char *)&text, "invoke : %dms\n\r",time_end_invoke - time_start_invoke);
			UART_Send_String((char*)text);

			HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)&pic, FrameWidth * FrameHeight * 2 / 4);
		}
		// HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 96;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
			|RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_4);
}

/* USER CODE BEGIN 4 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	static uint32_t count = 0,tick = 0;

	if(HAL_GetTick() - tick >= 1000)
	{
		tick = HAL_GetTick();
		Camera_FPS = count;
		count = 0;
	}
	count ++;

	DCMI_FrameIsReady = 1;
}

void Get_Data(void* target){
	int r_base = FrameHeight/2 - 28;
	int c_base = FrameWidth/2 - 28;
	float (*a_target)[28] = (float (*)[28])target;
	for(int r_t = 0;r_t<28;r_t++){
		for(int c_t = 0;c_t<28;c_t++){
			uint16_t pixel = pic[r_base+(2*r_t)][c_base+(2*c_t)];
			uint8_t r = ((pixel >> 11) & 0x1F)<<3;
			uint8_t g = ((pixel >> 5) & 0x3F)<<2;
			uint8_t b = (pixel & 0x1F)<<3;
			float gray = (3*r+6*g+b)/10./255.;

			a_target[r_t][c_t] = gray;
		}
	}
}

int Get_Top_Prediction(const float* predictions, int num_categories) {
	float max_score = predictions[0];
	int guess = 0;

	for (int category_index = 1; category_index < num_categories; category_index++) {
		const float category_score = predictions[category_index];
		if (category_score > max_score) {
			max_score = category_score;
			guess = category_index;
		}
	}
	return guess;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
	{
		LED_Blink(5, 250);
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

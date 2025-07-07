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

#include "models/hello_world_float_model_data.h"
#include "models/hello_world_int8_model_data.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
namespace {
	using HelloWorldOpResolver = tflite::MicroMutableOpResolver<1>;

	TfLiteStatus RegisterOps(HelloWorldOpResolver& op_resolver) {
		TF_LITE_ENSURE_STATUS(op_resolver.AddFullyConnected());
		return kTfLiteOk;
	}
}  // namespace
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
void Wifi_StateMachine(int top_ind);
void RX_DATA(void);
/* USER CODE BEGIN PFP */

// TFLM Hello World
TfLiteStatus ProfileMemoryAndLatency();
TfLiteStatus LoadFloatModelAndPerformInference();
TfLiteStatus LoadQuantModelAndPerformInference();

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

constexpr int tensor_arena_size = 400 * 1024;
static uint8_t tensor_arena[tensor_arena_size];
static bool wifi_ready = false;

int Get_Top_Prediction(const float* predictions, int num_categories);
void Get_Data(void* target);
void Get_Capture(void);

// Object detection functions
float sigmoid(float x);
struct BoundingBox {
    float x, y, w, h, confidence;
};
int process_predictions(const float* predictions, struct BoundingBox* boxes, float confidence_threshold);
int apply_nms(struct BoundingBox* boxes, int num_boxes, float nms_threshold);
float calculate_iou(const struct BoundingBox* box1, const struct BoundingBox* box2);
void draw_bounding_boxes(struct BoundingBox* boxes, int num_boxes);
void print_boxes_json(struct BoundingBox* boxes, int num_boxes);



int a=0;
typedef enum {
  STATE_INIT = 0,
  STATE_DHCP_ENABLE,
  STATE_JOIN_AP,
//  STATE_WAIT_JOIN,
//  STATE_GET_IP,
  STATE_START_TCP,
//  STATE_WAIT_TCP,
  STATE_SEND_LENGTH,
  STATE_SEND_DATA,
  STATE_FINAL,
  STATE_CHECK_AP
} WifiState;
WifiState wifi_state = STATE_INIT;

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
	MX_TIM5_Init();
	UART_Init();
	UART_Init2();
	/* USER CODE BEGIN 2 */
	uint8_t text[20];

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

	const tflite::Model* model = ::tflite::GetModel(g_hello_world_int8_model_data);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		for(;;);
	}

	UART_Send_String((char*)"TFLITE_SCHEMA_VERSION OK!\n\r");
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);

	tflite::MicroMutableOpResolver<11> micro_op_resolver;

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
	micro_op_resolver.AddRelu();

	tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size);
	if(interpreter.AllocateTensors() != kTfLiteOk){
		sprintf((char *)&text, "TFLM Allocate Tensor Failed!!");
		UART_Send_String((char*)text);
		while(1);
	}

	TfLiteTensor* input = interpreter.input(0);

	while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
	{
		sprintf((char *)&text, "Camera id:0x%x   ", hcamera.device_id);
		LCD_ShowString(0, 58, ST7735Ctx.Width, 16, 12, text);

		LED_Blink(5, 500);

		sprintf((char *)&text, "LongPress K1 to Run");
		LCD_ShowString(0, 58, ST7735Ctx.Width, 16, 12, text);

		LED_Blink(5, 500);
	}

//	ProfileMemoryAndLatency();
//	LoadFloatModelAndPerformInference();
//	LoadQuantModelAndPerformInference();
//
//	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)&pic, FrameWidth * FrameHeight * 2 / 4);


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	bool inferring = false;
	int top_ind=-1;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		ST7735_DrawVLine(&st7735_pObj, 160/2-32, 80/2-32, 64, 0xf800);
		ST7735_DrawVLine(&st7735_pObj, 160/2+32, 80/2-32, 64, 0xf800);
		ST7735_DrawHLine(&st7735_pObj, 160/2-32, 80/2-32, 64, 0xf800);
		ST7735_DrawHLine(&st7735_pObj, 160/2-32, 80/2+32, 64, 0xf800);

		bool button = (PIN1);

		if (button && !inferring && !wifi_ready) {
			inferring = true;
			wifi_ready = true;
			PIN1=0;
			HAL_DCMI_Stop(&hdcmi);
			DCMI_FrameIsReady = 0;

			int time_start_lcd = HAL_GetTick();
			ST7735_FillRGBRect(&st7735_pObj,0,0,(uint8_t *)&pic[20][0], ST7735Ctx.Width, 80);
			int time_end_lcd = HAL_GetTick();

			// Draw Input Data Guide Line


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
	        top_ind = Get_Top_Prediction(output->data.f, 8);


			sprintf((char *)&text, "%d %.1f%%\n\r", top_ind, output->data.f[top_ind] * 100.0);
			UART_Send_String((char*)text);


	        HAL_Delay(1000);


	        HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT,
	                           (uint32_t)&pic,
	                           FrameWidth * FrameHeight * 2 / 4);

	        inferring = false;

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
		else if (!button && !inferring) {
	        HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT,
	                           (uint32_t)&pic,
	                           FrameWidth * FrameHeight * 2 / 4);

	        if (DCMI_FrameIsReady) {
	            HAL_DCMI_Stop(&hdcmi);
	            DCMI_FrameIsReady = 0;
	            ST7735_FillRGBRect(&st7735_pObj, 0, 0,
	                               (uint8_t *)&pic[20][0],
	                               ST7735Ctx.Width, 80);
	        }
	    }

		if(wifi_ready){

			if(tim5_flag){
				tim5_flag = 0;
				Wifi_StateMachine(top_ind);
	//			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);  // 현재 상태 반전
	//			UART_Send_String((char*)"3sec");
			}
			if(uart2_rx_flag){
				uart2_rx_flag = 0;
				RX_DATA();
			}
		}
		// HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	}

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


uint16_t to_little_endian(uint16_t x) {
	return (x >> 8) | (x << 8);
}


void Get_Data(void* target) {
	float (*a_target)[64][3] = (float (*)[64][3])target;  // float[64][64][3]로 사용

	// 중앙 64x64 영역의 시작 좌표 계산
	int start_row = (FrameHeight - 64) / 2; // 120-64=56, 56/2=28
	int start_col = (FrameWidth - 64) / 2;  // 160-64=96, 96/2=48

	for (int r = 0; r < 64; r++) {
		for (int c = 0; c < 64; c++) {
			uint16_t pixel = pic[start_row + r][start_col + c];
			pixel = to_little_endian(pixel); // 빅엔디안 변환

			// RGB565에서 각 색상 추출
			uint8_t r5 = (pixel >> 11) & 0x1F;
			uint8_t g6 = (pixel >> 5) & 0x3F;
			uint8_t b5 = pixel & 0x1F;

			// 5비트/6비트 값을 8비트로 확장 (정확한 변환)
			uint8_t r8 = (r5 << 3) | (r5 >> 2); // 5비트 -> 8비트
			uint8_t g8 = (g6 << 2) | (g6 >> 4); // 6비트 -> 8비트
			uint8_t b8 = (b5 << 3) | (b5 >> 2); // 5비트 -> 8비트

			a_target[r][c][0] = r8 / 255.0f;
			a_target[r][c][1] = g8 / 255.0f;
			a_target[r][c][2] = b8 / 255.0f;
		}
	}
}

int Get_Top_Prediction(const float* predictions, int num_categories) {
	float max_score = predictions[0];
	int guess = 0;

	for (int category_index = 0; category_index <= num_categories; category_index++) {
		const float category_score = predictions[category_index];
		if (category_score > max_score) {
			max_score = category_score;
			guess = category_index;
		}
	}
	if (max_score < 0.95 || guess == 0) {
		return 9;
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
		//LED_Blink(5, 250);
		UART_Send_String((char*)"Error");
	}
	/* USER CODE END Error_Handler_Debug */
}

void Wifi_StateMachine(int top_ind) {
  switch (wifi_state) {
    case STATE_INIT:
    	wifi_state = STATE_DHCP_ENABLE;
    	break;

    case STATE_DHCP_ENABLE:
    	if(a==0) {
    		a=99;
    		UART_Send_String_wifi((char*)"AT+CWDHCP=1,1\r\n");
    	}
    	a=0;
    	wifi_state = STATE_JOIN_AP;
      break;

    case STATE_JOIN_AP:
    	if(a==0) {
    		a=99;
    		UART_Send_String_wifi((char*)"AT+CWJAP=\"Team3\",\"12345678q\"\r\n");
    	}
    	else if(a==1) {
    		a=0;
    		wifi_state = STATE_START_TCP;
		}
    	else if (a==2) {
    		a=0;
    	}
      break;

//    case STATE_WAIT_JOIN:
//      wifi_state = STATE_START_TCP;
//      break;

//    case STATE_GET_IP:
//      UART_Send_String_wifi("AT+CIPSTA?\r\n");
//      wifi_state = STATE_START_TCP;
//      break;

    case STATE_START_TCP:
    	if(a==0) {
    		a=99;
    		UART_Send_String_wifi((char*)"AT+CIPSTART=\"TCP\",\"192.168.137.156\",8000\r\n");
    	}
    	else if(a==1) {
    		a=0;
    		wifi_state = STATE_SEND_LENGTH;
    	}
    	else if(a==2) {
    		a=0;
    	}
    	break;

//    case STATE_WAIT_TCP:
//    	wifi_state = STATE_SEND_LENGTH;
//    	break;

    case STATE_SEND_LENGTH:
    	if(a==0) {
    		a=99;
    		UART_Send_String_wifi((char*)"AT+CIPSEND=1\r\n");
    	}
    	a=0;
    	wifi_state = STATE_SEND_DATA;
      break;

    case STATE_SEND_DATA:
    	if(a==0) {
    		a=99;
    		char buf=(char)top_ind;
//    		sprintf(buf, "%d\r\n", top_ind);
    		UART_Send_String_wifi(&buf);
    		wifi_ready =false;
    		wifi_state = STATE_INIT;
    		len=0;
    	}
    	else if(a==1) {
			a=0;

		}
      break;

//    case STATE_CHECK_AP:
//    	if(a==0) {
//    		a=99;
//    		UART_Send_String_wifi((char*)"AT+CWJAP?\r\n");
//    	}
//    	else if(a==2) {
//    		a=0;
//    		wifi_state = STATE_JOIN_AP;
//    	}
//    	else if(a==3) {
//    		a=0;
//    		wifi_state = STATE_START_TCP;
//    	}
//    	break;

    case STATE_FINAL:
//    	wifi_ready =false;
    	break;

    default:
      break;
  }
}

void RX_DATA(void) {
//	rxbuf[len++] = uart2_rx_data;
	//if(uart2_rx_data=='\n'){

		UART_Send_String(rxbuf);
	  switch (wifi_state) {
		case STATE_INIT:
			;
		  break;

		case STATE_DHCP_ENABLE:
			;
		  break;

		case STATE_JOIN_AP:
			if(strstr(rxbuf,"IP")) a=1;
			else if(strstr(rxbuf,"CWJAP")) a=2;
		  break;

//		case STATE_WAIT_JOIN:
//			;
//		  break;

//		case STATE_GET_IP:
//			;
//		  break;

		case STATE_START_TCP:
		  if(strstr(rxbuf,"CT")) a=1;
		  else if(strstr(rxbuf,"ROR")) a=2;
		  break;

//		case STATE_WAIT_TCP:
//			;
//		  break;

		case STATE_SEND_LENGTH:
			if(strstr(rxbuf,"OK")) a=1;;
		  break;

		case STATE_SEND_DATA:
//			if(strstr(rxbuf,"OK")) a=1;
		  break;

//		case STATE_CHECK_AP:
//			if(strstr(rxbuf,"AP")) a=2;
//			else a=3;
//		  break;

		case STATE_FINAL:
			;
		  break;

		default:
		  break;
	  }
	  len=0;
	//}
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

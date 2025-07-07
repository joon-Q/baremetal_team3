/**
  ******************************************************************************
  * @file    uart.c
  * @brief   This file provides code for the configuration
  *          of uart for debug.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

volatile int uart2_rx_flag;
volatile char uart2_rx_data;

UART_HandleTypeDef UartHandle;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart2_tx;

 char rxbuf[200] = {0};
 int len = 0;

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


void UART_Init(void){
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
	
	/* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  __HAL_RCC_USART1_CLK_ENABLE();
	
  UartHandle.Instance        = USART1;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

void UART_Init2(void){
  /* Enable USARTx clock */
  __HAL_RCC_USART2_CLK_ENABLE();

  huart1.Instance        = USART2;
  huart1.Init.BaudRate   = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits   = UART_STOPBITS_1;
  huart1.Init.Parity     = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart1.Init.Mode       = UART_MODE_TX_RX;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&huart1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  huart1.RxISR = USART2_Rx_ISR;
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	if(huart->Instance == USART2){
//		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
//		RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

//		__HAL_RCC_GPIOA_CLK_ENABLE();
//		__HAL_RCC_DMA1_CLK_ENABLE();
//
		hdma_usart2_tx.Instance = DMA1_Stream1;
		hdma_usart2_tx.Init.Request = DMA_REQUEST_USART2_TX;
		hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_tx.Init.Mode = DMA_NORMAL;
		hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(huart,hdmatx,hdma_usart2_tx);
//
		__HAL_DMA_ENABLE_IT(&hdma_usart2_tx,DMA_IT_TC);
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

		__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}


void UART_Send_Char(char ch){
	HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, HAL_MAX_DELAY); 
}

void UART_Send_String(char *str){
	HAL_UART_Transmit(&UartHandle, (uint8_t*)str, strlen(str), HAL_MAX_DELAY); 
}

void UART_Send_String_wifi(char *str){
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)str, strlen(str));
//	HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void USART2_Rx_ISR(UART_HandleTypeDef* huart){

	HAL_UART_Receive(huart, (uint8_t*)&uart2_rx_data, 1, HAL_MAX_DELAY);
	rxbuf[len++] = uart2_rx_data;
	if(uart2_rx_data='\n') {
		uart2_rx_flag = 1;
		rxbuf[len]='\0';

	}
}

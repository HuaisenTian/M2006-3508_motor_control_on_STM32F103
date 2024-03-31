/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "pid.h"
#include <string.h>
#include <stdlib.h>
// �����жϻص������ı�������������Ŀ��ֵ
#define RX_BUFFER_SIZE 16
char rxBuffer[RX_BUFFER_SIZE]; // ���ڽ��ջ�����
char rxString[RX_BUFFER_SIZE]; // �洢ʵ�ʽ��յ����ַ���
volatile uint8_t rxIndex = 0; // �����ַ����ĵ�ǰλ��
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  // ���������ж�
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxBuffer, 1);
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// �ض���printf
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	return ch;
}

// �ض���getchar
int fgetc(FILE *f)
{
	int ch;
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET);
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	return (ch);
}

// ���ڽ����ж�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *_huart)
{
    if(_huart->Instance == USART1)
    {
        if(rxBuffer[rxIndex] != '\n'){
            if(rxIndex < RX_BUFFER_SIZE - 1)
            {
                rxIndex++;
            }
        }
        else{
            memcpy(rxString, rxBuffer, rxIndex); // ���ƽ��յ����ַ���
            rxString[rxIndex] = '\0'; // ȷ���ַ�����ȷ����
            rxIndex = 0; // ����������׼�������µ��ַ���
            // �ж�����Ŀ��ֵ�Ƿ����
            long num = strtol(rxString, NULL, 10);
            if(num >= 0 && num <= 10000)
            {   
                // ����ֻ�޸��˵�һ�������Ŀ��ֵ
                target_reset(&motor_pid[0], (int16_t)num);
            }
            memset(rxBuffer, 0, RX_BUFFER_SIZE); // ��ս��ջ�������׼����һ�ν���
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxBuffer[rxIndex], 1); // �����������ڽ����ж�
    }
}
/* USER CODE END 1 */

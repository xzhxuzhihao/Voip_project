/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "gpio.h"
#include "main.h"
//#include "delay.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  //__HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  //__HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
    //HAL_GPIO_WritePin(RUN1_GPIO_Port, RUN1_Pin, GPIO_PIN_SET);
   // HAL_GPIO_WritePin(RST1_GPIO_Port, RST1_Pin, GPIO_PIN_RESET);
    //HAL_GPIO_WritePin(RST1_GPIO_Port, RST1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(SPI1_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
   // HAL_GPIO_WritePin(SPI2_GPIO_Port, SPI2_SCK_Pin, GPIO_PIN_SET);
   // HAL_GPIO_WritePin(SPI2_GPIO_Port, SPI2_SDIO_Pin, GPIO_PIN_SET);
  
  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(GPIOB, MCLKEN_Pin|BCLKEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RUN1_Pin */
  //GPIO_InitStruct.Pin = RUN1_Pin;
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(RUN1_GPIO_Port, &GPIO_InitStruct);
 
  /*Configure GPIO pins : RST1_Pin */
  //GPIO_InitStruct.Pin = RST1_Pin;
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(RST1_GPIO_Port, &GPIO_InitStruct);
  //delay_us(1);                              //低电平要大于500ns
  //HAL_GPIO_WritePin(RST1_GPIO_Port, RST1_Pin, GPIO_PIN_SET);
  //delay_ms(3);                              //电平拉高后等待3ms;  
  
  /*Configure GPIO pins : CALL1_Pin CALL2_Pin */
  //GPIO_InitStruct.Pin = CALL1_Pin|CALL2_Pin;
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(CALL_GPIO_Port, &GPIO_InitStruct);  

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = ADDR3_Pin|ADDR2_Pin|ADDR1_Pin|ADDR0_Pin;  //slot槽位号的管脚
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ADDR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  //GPIO_InitStruct.Pin = ADDRH3_Pin|ADDRH2_Pin|ADDRH1_Pin|ADDRH0_Pin;  //ADDRH3_Pin为FPGA片选,其余为模块选择
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  //HAL_GPIO_Init(ADDRH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  //GPIO_InitStruct.Pin = SPI2_SDIO_Pin|SPI2_SCK_Pin;               //与SHL1318连接，与SHL1318通信
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(SPI2_GPIO_Port, &GPIO_InitStruct); 

  //GPIO_InitStruct.Pin = Test_Pin;               //test
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(Test_GPIO_Port, &GPIO_InitStruct); 

  /*Configure GPIO pins : PAPin PAPin PAPin */
  //GPIO_InitStruct.Pin = SPI1_CS_Pin|SPI1_SDIO_Pin|SPI1_SCK_Pin;  //与FPGA通信
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  //HAL_GPIO_Init(SPI1_GPIO_Port, &GPIO_InitStruct);
  
  /*Configure GPIO pins : PAPin PAPin PAPin */
  //GPIO_InitStruct.Pin = INT1_Pin|INT3_Pin|INT4_Pin;              //外部中断管脚，连接SHL1318连接
  //GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(INT1_3_4_GPIO_Port, &GPIO_InitStruct);
  
  /*Configure GPIO pins : PAPin */
  //GPIO_InitStruct.Pin = INT2_Pin;                               ////外部中断管脚，连接SHL1318连接
  //GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(INT2_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */
/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   led应用函数接口
 
  ******************************************************************************
  */
  
uint8_t get_SLOT(void)
{
    uint8_t myslot;
	myslot = (HAL_GPIO_ReadPin(ADDR_GPIO_Port, ADDR3_Pin) & 0x01);
	myslot = (myslot << 1) + (HAL_GPIO_ReadPin(ADDR_GPIO_Port, ADDR2_Pin) & 0x01);
	myslot = (myslot << 1) + (HAL_GPIO_ReadPin(ADDR_GPIO_Port, ADDR1_Pin) & 0x01);
	myslot = (myslot << 1) + (HAL_GPIO_ReadPin(ADDR_GPIO_Port, ADDR0_Pin) & 0x01);
    return myslot;
}


/* USER CODE END 2 */

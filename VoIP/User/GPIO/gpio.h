/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

//引脚定义
/*******************************************************/

#define ADDR3_Pin                     GPIO_PIN_8
#define ADDR2_Pin                     GPIO_PIN_11
#define ADDR1_Pin                     GPIO_PIN_12 
#define ADDR0_Pin                     GPIO_PIN_11 
#define ADDR_GPIO_Port                GPIOC                      
#define ADDR_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()


/************************************************************/

/* 定义控制IO的宏 */
//#define RUNLED_TOGGLE()		digitalToggle(RUN1_GPIO_Port,RUN1_Pin)
#define RUN1_TOGGLE()       digitalToggle(RUN1_GPIO_Port, RUN1_Pin)
#define CALL1_TOGGLE()      digitalToggle(CALL_GPIO_Port, CALL1_Pin)
#define CALL2_TOGGLE()      digitalToggle(CALL_GPIO_Port, CALL2_Pin)
/*#define INT1_TOGGLE()       digitalToggle(INT1_3_4_GPIO_Port, INT1_Pin)
#define INT2_TOGGLE()       digitalToggle(INT2_GPIO_Port, INT2_Pin)
#define INT3_TOGGLE()       digitalToggle(INT1_3_4_GPIO_Port, INT3_Pin)
#define INT4_TOGGLE()       digitalToggle(INT1_3_4_GPIO_Port, INT4_Pin)*/

#define RST1_TOGGLE()       digitalToggle(RST1_GPIO_Port, RST1_Pin)

#define SPI2_SCK_TOGGLE()   digitalToggle(SPI2_GPIO_Port,SPI2_SCK_Pin)
#define SPI2_SDIO_TOGGLE()  digitalToggle(SPI2_GPIO_Port,SPI2_SDIO_Pin) //HAL_GPIO_TogglePin(SPI2_GPIO_Port,SPI2_SDIO_Pin) //

#define ADDRH0(a)           HAL_GPIO_WritePin(ADDRH_GPIO_Port,ADDRH0_Pin,a)
#define ADDRH1(a)           HAL_GPIO_WritePin(ADDRH_GPIO_Port,ADDRH1_Pin,a)
#define ADDRH2(a)           HAL_GPIO_WritePin(ADDRH_GPIO_Port,ADDRH2_Pin,a)
#define ADDRH3(a)           HAL_GPIO_WritePin(ADDRH_GPIO_Port,ADDRH3_Pin,a)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
//extern uint8_t MySlot;

uint8_t get_SLOT(void);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


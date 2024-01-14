/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "debug.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h" 
#include "./eth/bsp_eth.h" 

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

typedef struct frame{
    uint8_t HeadH;
    uint8_t HeadL;
    uint8_t Length;
	uint8_t slot; 
	uint8_t card_type;
	uint8_t MSG_No;
	uint8_t WRMode;
	uint8_t StartAddrH;
	uint8_t StartAddrL;
	uint8_t DatLenH;
	uint8_t DatLenL;
	uint8_t Data[16];
	uint8_t Tail;	
}frame_typedef;

typedef struct Err{
	uint8_t ErrFlag;     //                                          MOV	ErrFlag, #Err_RX1ADDRESS
	uint8_t ErrMsg;      //USART1_RX_Buff[5];    //MsgSi1;          //MOV	ErrMsg, MsgSi1
	uint8_t ErrAddrH;    //USART1_RX_Buff[7];    //DAddrHSi1;       //MOV	ErrAddrH, DAddrHSi1
	uint8_t ErrAddrL;    //USART1_RX_Buff[8];    //DAddrLSi1;       //MOV	ErrAddrL, DAddrLSi1
	uint8_t ErrLenH;     //USART1_RX_Buff[9];    //DAllLenHSi1;     //MOV	ErrLenH, DAllLenHSi1
	uint8_t ErrLenL;     //USART1_RX_Buff[10];   //DAllLenLSi1;     //MOV	ErrLenL, DAllLenLSi1	

}Err_typedef;

void Error_Handler(void);

void BSP_Init(void);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

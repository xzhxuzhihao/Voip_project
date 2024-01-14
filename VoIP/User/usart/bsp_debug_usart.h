#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

#define USART1_Rcvd_MaxLen                      0x30
#define USART1_Rcvd_Cmplt                       0x8000
#define USART1_Rcvd_FrmVerified                 0x4000
#define MaxFrmLen                               0x20
#define MinFrmLen                               0x0C
#define UsartSendStart                          0x00
#define UsartSendEnd                            0x01

#define FrmHeadH_Down                           0x0B
#define FrmHeadL_Down                           0x0E
#define FrmHeadH_Up                             0x0B
#define FrmHeadL_Up                             0x0D

#define FrmTail1         0x0D
#define FrmTail2         0x8D
//串口波特率
#define DEBUG_USART_BAUDRATE                    9600

//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __USART1_CLK_ENABLE();

#define RCC_PERIPHCLK_UARTx                     RCC_PERIPHCLK_USART1
#define RCC_UARTxCLKSOURCE_SYSCLK               RCC_USART1CLKSOURCE_SYSCLK

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
#define DEBUG_USART_RX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
#define DEBUG_USART_TX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn

#define DEBUG_USART_TXD1_ACT_PIN                  GPIO_PIN_8                 
#define DEBUG_USART_TXD1_ACT_GPIO_PORT            GPIOE                      
#define DEBUG_USART_TXD1_ACT_GPIO_CLK_ENABLE()    __GPIOE_CLK_ENABLE()

#define DEBUG_USART_RXD1_ACT_PIN                  GPIO_PIN_9                 
#define DEBUG_USART_RXD1_ACT_GPIO_PORT            GPIOE                      
#define DEBUG_USART_RXD1_ACT_GPIO_CLK_ENABLE()    __GPIOE_CLK_ENABLE()

#define DigitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
#define DEBUG_USART_TXD1_ACT_TOGGLE		DigitalToggle(DEBUG_USART_TXD1_ACT_GPIO_PORT,DEBUG_USART_TXD1_ACT_PIN)
#define DEBUG_USART_RXD1_ACT_TOGGLE		DigitalToggle(DEBUG_USART_RXD1_ACT_GPIO_PORT,DEBUG_USART_RXD1_ACT_PIN)
/************************************************************/

void Usart_SendString(uint8_t *str);
void Usart_SendFrame(uint8_t *str,uint8_t len);
void Usart_SendFrame_IT(uint8_t *str,uint8_t len);
void DEBUG_USART_Config(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */

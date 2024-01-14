/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f429xx.h" 
#include "./usart/bsp_debug_usart.h"
#include "main.h"
#include "gpio.h"

UART_HandleTypeDef UartHandle;
frame_typedef tx_frame,rx_frame;

static uint8_t USART1_Rcvd_Byte;
static uint8_t USART1_Rcvd_Buffer[USART1_Rcvd_MaxLen];
static	uint8_t  FrmLength = MaxFrmLen;
volatile uint8_t UsartSendFlag;
uint16_t USART1_Rcvd_State;
extern uint8_t MySlot;  

volatile 	uint8_t* pframe;
volatile 	uint8_t TxXferCount;
 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void DEBUG_USART_Config(void)
{ 
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef  GPIO_InitStruct; 
	
  /*开启LED相关的GPIO外设时钟*/
  DEBUG_USART_TXD1_ACT_GPIO_CLK_ENABLE();
  DEBUG_USART_RXD1_ACT_GPIO_CLK_ENABLE();

  /*选择要控制的GPIO引脚*/															   
  GPIO_InitStruct.Pin = DEBUG_USART_TXD1_ACT_PIN;	

  /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

  /*设置引脚为上拉模式*/
  GPIO_InitStruct.Pull  = GPIO_PULLUP;

  /*设置引脚速率为高速 */   
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
  HAL_GPIO_Init(DEBUG_USART_TXD1_ACT_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DEBUG_USART_RXD1_ACT_PIN;
  HAL_GPIO_Init(DEBUG_USART_RXD1_ACT_GPIO_PORT, &GPIO_InitStruct);

  UartHandle.Instance          = DEBUG_USART;
  
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle); 
  /*函数 HAL_UART_Init 内部会调用串口使能函数使能相应串口，所以调用了该函数之后我们就不需要重复使能串口了。
    串口作为一个重要外设，在调用的初始化函数 HAL_UART_Init 内部，会先调用 MSP 初始化回调函数
	void HAL_UART_MspInit(UART_HandleTypeDef *huart)进行 MCU 相关的初始化*/
  if (HAL_UART_Init(&UartHandle) != HAL_OK) 
  {
    Error_Handler();
  }
	
}


/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  DEBUG_USART_CLK_ENABLE();
	
  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //使能USART1中断通道  
}


/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	//DEBUG_USART_TXD1_ACT_TOGGLE;
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	//DEBUG_USART_RXD1_ACT_TOGGLE;
	return (ch);
}
/*************** 发送帧*\*********************/
void Usart_SendFrame(uint8_t *str,uint8_t len)
{
    unsigned int k=0;
    do {
        HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k) ,1,1000);
        k++;
    } while (k<len);

}
/*************** 发送帧____中断方式*\*********************/
void Usart_SendFrame_IT(uint8_t *str,uint8_t len)
{
    //是否需要HAL_GPIO_WritePin(USART1_TX_GPIO_PORT,USART1_TX_PIN,GPIO_PIN_RESET);
	
	//HAL_UART_Transmit_IT( &huart1,(uint8_t *)str,(uint16_t)len);
	
	pframe = str;
	TxXferCount = len;
	/* Enable the Transmit Data Register Empty interrupt */
	//ATOMIC_SET_BIT(USART1->CR1, USART_CR1_TXEIE);
	USART1->CR1 |= USART_CR1_TXEIE;
	
	//是否需要HAL_GPIO_WritePin(USART1_TX_GPIO_PORT,USART1_TX_PIN,GPIO_PIN_SET);
}

void USART1_IRQHandler(void)                	
{ 
    uint32_t isrflags   = USART1->SR;
	uint32_t cr1its     = USART1->CR1;

	//接收中断
	if(__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_RXNE) != RESET)
	{
#if TEST
		Rcvd_Byte = (uint8_t)(USART1->RDR & (uint8_t)0x00FF);  //(uint8_t)(huart1.Instance->RDR & (uint8_t)0x00FF);
#else
		
		HAL_UART_Receive(&UartHandle,&USART1_Rcvd_Byte,1,1000);
#endif
		//将数据放入缓冲区
		if(((USART1_Rcvd_State & USART1_Rcvd_Cmplt) != USART1_Rcvd_Cmplt))
		{
		    if(USART1_Rcvd_State < USART1_Rcvd_MaxLen)
		    {
			    USART1_Rcvd_Buffer[USART1_Rcvd_State] = USART1_Rcvd_Byte;
			    USART1_Rcvd_State++;
		    }
            else
			{
			    USART1_Rcvd_State = 0x00;       //或者设置错误标志
				
			}				
		    __HAL_UART_CLEAR_FLAG(&UartHandle,UART_FLAG_RXNE);
			DEBUG_USART_RXD1_ACT_TOGGLE;
	    }
	}
	//空闲中断
	if(__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_IDLE) != RESET)
	{
		//一帧数据接收完成
		//USART1_IdleCallback(USART1_RX_Buff,USART_RX_STA);
		//USART1_Rcvd_State |= Rcv_Locked;
		USART1_Rcvd_State |= USART1_Rcvd_Cmplt;
		__HAL_UART_CLEAR_IDLEFLAG(&UartHandle);
		//DEBUG_USART_RXD1_ACT_TOGGLE;
	}
#if TEST
   /* UART in mode Transmitter ------------------------------------------------*/
   if ((isrflags & USART_ISR_TXE) != RESET)      
   {
		if (TxXferCount == RESET)
		{
		  /* Disable the UART Transmit Data Register Empty Interrupt */
		  //ATOMIC_CLEAR_BIT(USART1->CR1, USART_CR1_TXEIE);
			USART1->CR1 &= ~USART_CR1_TXEIE;

		  /* Enable the UART Transmit Complete Interrupt */
		  //ATOMIC_SET_BIT(USART1->CR1, USART_CR1_TCIE);
			USART1->CR1 |= USART_CR1_TCIE;
		}
		else
		{
		  USART1->TDR = (uint8_t) *pframe;
		  pframe++;
		  TxXferCount--;
		}
   }

   /* UART in mode Transmitter (transmission end) -----------------------------*/
   if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
   {
      UsartSendFlag = UsartSendEnd;
	   /* Disable the UART Transmit Complete Interrupt */
      //ATOMIC_CLEAR_BIT(USART1->CR1, USART_CR1_TCIE);
	  USART1->CR1 &= ~USART_CR1_TCIE; 
	}
#else
	HAL_UART_IRQHandler(&UartHandle);
#endif	
}

uint8_t RxVerify(void)
{
	uint8_t i,bit,result;
	
	if((USART1_Rcvd_State & USART1_Rcvd_Cmplt)!= USART1_Rcvd_Cmplt)   //非锁定状态，串口还要接收数据			
		return 0x00;

		//已经接收完整的帧	
	FrmLength = USART1_Rcvd_Buffer[2];
	if((FrmLength < MinFrmLen) ||(FrmLength > MaxFrmLen )) //接收帧长度不对
	{
		USART1_Rcvd_State = 0x00;
		return 0x01;
	}
	result = 0;
	for(i = 0;i < FrmLength; i++)
    {	
		for(bit=0;bit<8;bit++)
              result += (USART1_Rcvd_Buffer[i]>>bit) & 0x01;
		result &= 0x01;           //判断一字节中，1的个数的奇偶性. result是零则为偶数；是1，则为奇数
    }
	//result = result & 0x01;       //bit0是否是零，是零则为偶数；是1，则为奇数 
	if(result == 0x00) //接收了完整正确的一帧
	{
	   USART1_Rcvd_State |= USART1_Rcvd_FrmVerified;
	   return 0x02;
	}		//
	else   //接收帧校验错误
	{
		USART1_Rcvd_State = 0x0000;
		FrmLength = MaxFrmLen;
        return 0x03;		
	}
	//以下调试用	  
	//RUNLED_TOGGLE;
	//USART_RX_STA = 0;		
}


uint8_t Rx_UnderStand(void)     //发现给MySlot槽位的信息
{
	uint8_t i,DataStart,DataEnd;
	if((USART1_Rcvd_State & USART1_Rcvd_FrmVerified) != USART1_Rcvd_FrmVerified)   //接收帧未验证，则返回，不对帧进行解析			
		return 0x00;
	if(USART1_Rcvd_Buffer[0] != FrmHeadH_Down)
	{
		USART1_Rcvd_State = 0x0000;
		FrmLength = MaxFrmLen;
        return   0x01;	    
	}
	if(USART1_Rcvd_Buffer[1] != FrmHeadL_Down)
	{
		USART1_Rcvd_State = 0x0000;
		FrmLength = MaxFrmLen;
        return 0x02;	    
	}	
	FrmLength = USART1_Rcvd_Buffer[2];
	if(USART1_Rcvd_Buffer[3] != MySlot)   //不是发给本槽位的帧
	{
		USART1_Rcvd_State = 0x0000;
		FrmLength = MaxFrmLen;
        return 0x03;	    
	}
	else
	{
		rx_frame.HeadH      = USART1_Rcvd_Buffer[0];
		rx_frame.HeadL      = USART1_Rcvd_Buffer[1];
		rx_frame.Length     = USART1_Rcvd_Buffer[2];
		rx_frame.slot       = USART1_Rcvd_Buffer[3];
		rx_frame.card_type  = USART1_Rcvd_Buffer[4];
		rx_frame.MSG_No     = USART1_Rcvd_Buffer[5];
		rx_frame.WRMode     = USART1_Rcvd_Buffer[6];
		rx_frame.StartAddrH = USART1_Rcvd_Buffer[7];
		rx_frame.StartAddrL = USART1_Rcvd_Buffer[8];
		rx_frame.DatLenH    = USART1_Rcvd_Buffer[9];
		rx_frame.DatLenL    = USART1_Rcvd_Buffer[10];

		if(rx_frame.Length > 0x0C)
		{
			DataStart = 0x00;
			DataEnd   = rx_frame.Length - 0x0B;		    
			for(i = DataStart; i < DataEnd; i++)
		    {
		        rx_frame.Data[i] = USART1_Rcvd_Buffer[i + 0x0B];
		    }
	    }
		rx_frame.Tail  = USART1_Rcvd_Buffer[rx_frame.Length - 0x01];
	    return 0x04;	
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &UartHandle)
	{
	    UsartSendFlag = UsartSendEnd;
		//HAL_UART_Receive_IT(&huart1, &Rcvd_Byte, 1);
	}
}

/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ�ô���1���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void DEBUG_USART_Config(void)
{ 
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef  GPIO_InitStruct; 
	
  /*����LED��ص�GPIO����ʱ��*/
  DEBUG_USART_TXD1_ACT_GPIO_CLK_ENABLE();
  DEBUG_USART_RXD1_ACT_GPIO_CLK_ENABLE();

  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStruct.Pin = DEBUG_USART_TXD1_ACT_PIN;	

  /*�������ŵ��������Ϊ�������*/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

  /*��������Ϊ����ģʽ*/
  GPIO_InitStruct.Pull  = GPIO_PULLUP;

  /*������������Ϊ���� */   
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
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
  /*���� HAL_UART_Init �ڲ�����ô���ʹ�ܺ���ʹ����Ӧ���ڣ����Ե����˸ú���֮�����ǾͲ���Ҫ�ظ�ʹ�ܴ����ˡ�
    ������Ϊһ����Ҫ���裬�ڵ��õĳ�ʼ������ HAL_UART_Init �ڲ������ȵ��� MSP ��ʼ���ص�����
	void HAL_UART_MspInit(UART_HandleTypeDef *huart)���� MCU ��صĳ�ʼ��*/
  if (HAL_UART_Init(&UartHandle) != HAL_OK) 
  {
    Error_Handler();
  }
	
}


/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
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
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //ʹ��USART1�ж�ͨ��  
}


/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	//DEBUG_USART_TXD1_ACT_TOGGLE;
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	//DEBUG_USART_RXD1_ACT_TOGGLE;
	return (ch);
}
/*************** ����֡*\*********************/
void Usart_SendFrame(uint8_t *str,uint8_t len)
{
    unsigned int k=0;
    do {
        HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k) ,1,1000);
        k++;
    } while (k<len);

}
/*************** ����֡____�жϷ�ʽ*\*********************/
void Usart_SendFrame_IT(uint8_t *str,uint8_t len)
{
    //�Ƿ���ҪHAL_GPIO_WritePin(USART1_TX_GPIO_PORT,USART1_TX_PIN,GPIO_PIN_RESET);
	
	//HAL_UART_Transmit_IT( &huart1,(uint8_t *)str,(uint16_t)len);
	
	pframe = str;
	TxXferCount = len;
	/* Enable the Transmit Data Register Empty interrupt */
	//ATOMIC_SET_BIT(USART1->CR1, USART_CR1_TXEIE);
	USART1->CR1 |= USART_CR1_TXEIE;
	
	//�Ƿ���ҪHAL_GPIO_WritePin(USART1_TX_GPIO_PORT,USART1_TX_PIN,GPIO_PIN_SET);
}

void USART1_IRQHandler(void)                	
{ 
    uint32_t isrflags   = USART1->SR;
	uint32_t cr1its     = USART1->CR1;

	//�����ж�
	if(__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_RXNE) != RESET)
	{
#if TEST
		Rcvd_Byte = (uint8_t)(USART1->RDR & (uint8_t)0x00FF);  //(uint8_t)(huart1.Instance->RDR & (uint8_t)0x00FF);
#else
		
		HAL_UART_Receive(&UartHandle,&USART1_Rcvd_Byte,1,1000);
#endif
		//�����ݷ��뻺����
		if(((USART1_Rcvd_State & USART1_Rcvd_Cmplt) != USART1_Rcvd_Cmplt))
		{
		    if(USART1_Rcvd_State < USART1_Rcvd_MaxLen)
		    {
			    USART1_Rcvd_Buffer[USART1_Rcvd_State] = USART1_Rcvd_Byte;
			    USART1_Rcvd_State++;
		    }
            else
			{
			    USART1_Rcvd_State = 0x00;       //�������ô����־
				
			}				
		    __HAL_UART_CLEAR_FLAG(&UartHandle,UART_FLAG_RXNE);
			DEBUG_USART_RXD1_ACT_TOGGLE;
	    }
	}
	//�����ж�
	if(__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_IDLE) != RESET)
	{
		//һ֡���ݽ������
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
	
	if((USART1_Rcvd_State & USART1_Rcvd_Cmplt)!= USART1_Rcvd_Cmplt)   //������״̬�����ڻ�Ҫ��������			
		return 0x00;

		//�Ѿ�����������֡	
	FrmLength = USART1_Rcvd_Buffer[2];
	if((FrmLength < MinFrmLen) ||(FrmLength > MaxFrmLen )) //����֡���Ȳ���
	{
		USART1_Rcvd_State = 0x00;
		return 0x01;
	}
	result = 0;
	for(i = 0;i < FrmLength; i++)
    {	
		for(bit=0;bit<8;bit++)
              result += (USART1_Rcvd_Buffer[i]>>bit) & 0x01;
		result &= 0x01;           //�ж�һ�ֽ��У�1�ĸ�������ż��. result������Ϊż������1����Ϊ����
    }
	//result = result & 0x01;       //bit0�Ƿ����㣬������Ϊż������1����Ϊ���� 
	if(result == 0x00) //������������ȷ��һ֡
	{
	   USART1_Rcvd_State |= USART1_Rcvd_FrmVerified;
	   return 0x02;
	}		//
	else   //����֡У�����
	{
		USART1_Rcvd_State = 0x0000;
		FrmLength = MaxFrmLen;
        return 0x03;		
	}
	//���µ�����	  
	//RUNLED_TOGGLE;
	//USART_RX_STA = 0;		
}


uint8_t Rx_UnderStand(void)     //���ָ�MySlot��λ����Ϣ
{
	uint8_t i,DataStart,DataEnd;
	if((USART1_Rcvd_State & USART1_Rcvd_FrmVerified) != USART1_Rcvd_FrmVerified)   //����֡δ��֤���򷵻أ�����֡���н���			
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
	if(USART1_Rcvd_Buffer[3] != MySlot)   //���Ƿ�������λ��֡
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

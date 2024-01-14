/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Frame_Responce.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __FRAME_RESPONCE_H__
#define __FRAME_RESPONCE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

/*��Ϣ�ţ�����MSG_No��ֵ*/
#define MSGA_IsChange	0x00      //MSGA_00   EQU	00H		; �Ƿ�仯	00H
#define MSGA_STYLE		0x01      //MSGA_STYLE		EQU	01H		; ���Ͱ汾	01H
#define MSGA_STATE		0x02      //MSGA_STATE		EQU	02H		; ״̬		02H
#define MSGA_STRU		0x02      //MSGA_STRU		EQU	02H		; �豸�ṹ	02H
#define MSGA_WARN		0x03      //MSGA_WARN		EQU	03H		; �澯		03H
#define MSGA_CHAR		0x04      //MSGA_CHAR		EQU	04H		; ҵ��		04H

#define MSGA_MALM		0x05      //MSGA_MALM		EQU	05H		; �澯����	05H
#define MSGA_PARA		0x06      //MSGA_PARA		EQU	06H		; ��������	06H
#define MSGA_E1		    0x07      //MSGA_E1		EQU	07H		; E1����	07H
#define MSGA_TS		    0x08      //MSGA_TS		EQU	08H		; ʱ϶����	08H
#define MSGA_DXC		0x09      //MSGA_DXC		EQU	09H		; ��������	09H

#define MSGA_Err		0x0A      //MSGA_Err		EQU	0AH		; ���󷵻�	0AH

#define MSGA_RST		0x10      //MSGA_RST		EQU	10H		; ��λ		10H
#define MSGA_HardWare	0x11      //MSGA_HardWare	EQU	11H		; Ӳ����Ϣ	11H
#define MSGA_UpLoad		0x20      //MSGA_UpLoad		EQU	20H		; �澯�ϱ�	0BH

/**/
#define MyCardType          0xA0                    //������
#define MyCardTypeVersion   0x11                    //�̰汾 V1.1

/*����*/
#define CHANGED		 0xFF		//; ��������Ϣ�仯,���Ǹ澯,�����շ�״̬������
#define CHANGEWARN	 0xF0		//; �澯�仯��
#define NOCHANGE	 0x00		//; û�б仯

#define Warn_num        0x22    //34��  32���绰�ӿڣ�2����������
#define Service_num     0x20    //32���绰�ӿ� 
#define MALM_num        0x00
#define Para_num        0x40   //�绰����ģʽ��32��(��4λ���������4λ��������)���绰����(���߷�ʽ)��32��
#define busE1_num       0x02   //E1�������ã�2��
#define TS_num          0x00
#define DXC_num         0x00
#define END_num         (Warn_num+Service_num +MALM_num+Para_num+TS_num+DXC_num)

#define WARN_ADDRH      0x00        //; �澯�������ʼ��ַ  �߰�λ            �߰�λ �Ͱ�λ      �߰�λ �Ͱ�λ
#define WARN_ADDRL      0x00        //; �澯�������ʼ��ַ  �Ͱ�λ          0x  00    00      0x  00    21     3����Ϣ
#define SERVICE_ADDRH   0x00        //; ҵ��������ʼ��ַ  �߰�λ          
#define SERVICE_ADDRL	0x22		//; ҵ��������ʼ��ַ  �Ͱ�λ          0x  00    22      0x  00    41     4����Ϣ
#define MALM_ADDRH		0x00		//; �澯���ε������ʼ��ַ  �߰�λ
#define MALM_ADDRL		0x42		//; �澯���ε������ʼ��ַ  �Ͱ�λ       0x  00   42       0x   00    42    5����Ϣ 
#define PARA_ADDRH		0x00		//; �����������ʼ��ַ  �߰�λ
#define PARA_ADDRL		0x42		//; �����������ʼ��ַ  �Ͱ�λ           0x  00   42      0x   00    81     6����Ϣ
#define busE1_ADDRH		0x00		//; E1�������ʼ��ַ  �߰�λ
#define busE1_ADDRL		0x82		//; E1�������ʼ��ַ  �Ͱ�λ             0x  00   82      0x   00    83    7����Ϣ
#define TS_ADDRH		0x00		//; TS�������ʼ��ַ  �߰�λ
#define TS_ADDRL		0x84		//; TS�������ʼ��ַ  �Ͱ�λ             0x  00   84                       8����Ϣ
#define DXC_ADDRH		0x00		//; ����������ʼ��ַ  �߰�λ
#define DXC_ADDRL		0x84		//; ����������ʼ��ַ  �Ͱ�λ           0x  00   84
#define END_ADDRH		0x00		//; ������Ϣ���ܸ���
#define END_ADDRL		0x84		//;

//--------------------- Error FLAG -------------------------
#define Return_OK		   0x00		//; û�д���

#define Err_MCUINRAM	   0x01		// 
#define Err_MCUEXRAM	   0x02		// 
#define Err_MCUBUS		   0x03		// 
#define Err_EEPROMPROTECT  0x04 	//
#define Err_EEPROMRW	   0x05 	// 
#define Err_EEPROMFORMAT   0x06		//
#define Err_DXC		       0x07		// 

#define Err_NETADDR		   0x10		// 
#define Err_DEVADDR		   0x11		// 
#define Err_DEVTYPE		   0x12		// 
#define Err_DEVVERSION	   0x13		// 
#define Err_SLOTADDR	   0x14		// 
#define Err_SPOOLTYPE	   0x15		// 
#define Err_CARDTYPE	   0x16		// 
#define Err_SPOOLVERSION   0x17		// 
#define Err_SPOOLSTATE	   0x18		// 
#define Err_SPOOLF1		   0x19		// 
#define Err_SPOOLF2		   0x1A		// 
#define Err_SPOOLADDR	   0x1B		// 
#define Err_SPOOLTAIL	   0x1C		// 
#define Err_INVALIDPAGE	   0x1D		// 
#define Err_NOFREESPACE	   0x1E		// 
#define Err_FPGALOAD	   0x1F		// 
#define Err_FPGARW		   0x20		// 

#define Err_SI1LOOP		   0x30		// 
#define Err_RX1VERIFY	   0x31		// 
#define Err_TX1VERIFY	   0x32		// 
#define Err_RX1UNDERSTAND  0x33		// 
#define Err_RX1SLOT		   0x34		// 
#define Err_RX1FRMLENGTH   0x35		// 
#define Err_RX1SPOOLTYPE   0x36		// 
#define Err_RX1MSG		   0x37		// 
#define Err_RX1RWMODE	   0x38		// 
#define Err_RX1ADDRESS     0x39     //Err_RX1ADDRESS	   0x39H
#define Err_RX1DATALENGTH  0x3A		// 
#define Err_RX1TAIL		   0x3B		// 

#define Err_SI2LOOP		   0x50		// 
#define Err_RX2VERIFY	   0x51		// 
#define Err_TX2VERIFY	   0x52		// 
#define Err_RX2UNDERSTAND  0x53		// 
#define Err_RX2SLOT		   0x54		// 
#define Err_RX2FRMLENGTH   0x55		// 
#define Err_RX2SPOOLTYPE   0x56		// 
#define Err_RX2MSG		   0x57		// 
#define Err_RX2RWMODE	   0x58		// 
#define Err_RX2ADDRESS	   0x59		// 
#define Err_RX2DATALENGTH  0x5A		// 
#define Err_RX2TAIL		   0x5B		// 

//ҵ������
#define OP_NONE             0xFF          //��ҵ��
#define OP_E1               0x01          //E1
#define OP_10BaseT		    0x02		  //10BaseT
#define OP_V35		        0x03		  //V.35
#define OP_RS232		    0x04 		  //RS232
#define OP_CHU		        0x05 		  //CHU,��·�ӿ�
#define OP_FXO		        0x06 		  //FXO,�ֶ����� SHL1218
#define OP_FXS		        0x07 		  //FXS,Զ������ SHL1318
#define OP_EM			    0x08 		  //E/M,E.M.����
#define OP_CAR		        0x09 		  //�ز���
#define OP_RS485		    0x0A 		  //RS485
#define OP_Optical			0x0B		  //��ӿ�
#define OP_MAC		        0x0C 		  //��ʯ��


//;------------------------ MODE --------------------------
#define MODEREAD    0x01               //MODEREAD		EQU	01H		; constant
#define MODEWRITE   0x02               //MODEWRITE		EQU	02H		; constant


//void PrepareTx(void);
//void TxVerify(void);
void Frame_Response(void);

void testframe(void);
void rx1_stateframe(void);
void rx2_stateframe(void);
void tx1_stateframe(void);
void tx2_stateframe(void);
			
#ifdef __cplusplus
}
#endif

#endif /* __FRAME_RESPONCE_H__ */

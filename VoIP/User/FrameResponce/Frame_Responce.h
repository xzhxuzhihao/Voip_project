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

/*消息号，变量MSG_No的值*/
#define MSGA_IsChange	0x00      //MSGA_00   EQU	00H		; 是否变化	00H
#define MSGA_STYLE		0x01      //MSGA_STYLE		EQU	01H		; 类型版本	01H
#define MSGA_STATE		0x02      //MSGA_STATE		EQU	02H		; 状态		02H
#define MSGA_STRU		0x02      //MSGA_STRU		EQU	02H		; 设备结构	02H
#define MSGA_WARN		0x03      //MSGA_WARN		EQU	03H		; 告警		03H
#define MSGA_CHAR		0x04      //MSGA_CHAR		EQU	04H		; 业务		04H

#define MSGA_MALM		0x05      //MSGA_MALM		EQU	05H		; 告警屏蔽	05H
#define MSGA_PARA		0x06      //MSGA_PARA		EQU	06H		; 参数配置	06H
#define MSGA_E1		    0x07      //MSGA_E1		EQU	07H		; E1配置	07H
#define MSGA_TS		    0x08      //MSGA_TS		EQU	08H		; 时隙配置	08H
#define MSGA_DXC		0x09      //MSGA_DXC		EQU	09H		; 交叉配置	09H

#define MSGA_Err		0x0A      //MSGA_Err		EQU	0AH		; 错误返回	0AH

#define MSGA_RST		0x10      //MSGA_RST		EQU	10H		; 复位		10H
#define MSGA_HardWare	0x11      //MSGA_HardWare	EQU	11H		; 硬件信息	11H
#define MSGA_UpLoad		0x20      //MSGA_UpLoad		EQU	20H		; 告警上报	0BH

/**/
#define MyCardType          0xA0                    //盘类型
#define MyCardTypeVersion   0x11                    //盘版本 V1.1

/*变量*/
#define CHANGED		 0xFF		//; 其他的信息变化,不是告警,比如收发状态、铃流
#define CHANGEWARN	 0xF0		//; 告警变化了
#define NOCHANGE	 0x00		//; 没有变化

#define Warn_num        0x22    //34个  32个电话接口，2个背板总线
#define Service_num     0x20    //32个电话接口 
#define MALM_num        0x00
#define Para_num        0x40   //电话信令模式：32个(高4位发送信令，第4位接收信令)，电话配置(热线方式)：32个
#define busE1_num       0x02   //E1总线配置：2个
#define TS_num          0x00
#define DXC_num         0x00
#define END_num         (Warn_num+Service_num +MALM_num+Para_num+TS_num+DXC_num)

#define WARN_ADDRH      0x00        //; 告警的相对起始地址  高八位            高八位 低八位      高八位 低八位
#define WARN_ADDRL      0x00        //; 告警的相对起始地址  低八位          0x  00    00      0x  00    21     3号消息
#define SERVICE_ADDRH   0x00        //; 业务的相对起始地址  高八位          
#define SERVICE_ADDRL	0x22		//; 业务的相对起始地址  低八位          0x  00    22      0x  00    41     4号消息
#define MALM_ADDRH		0x00		//; 告警屏蔽的相对起始地址  高八位
#define MALM_ADDRL		0x42		//; 告警屏蔽的相对起始地址  低八位       0x  00   42       0x   00    42    5号消息 
#define PARA_ADDRH		0x00		//; 参数的相对起始地址  高八位
#define PARA_ADDRL		0x42		//; 参数的相对起始地址  低八位           0x  00   42      0x   00    81     6号消息
#define busE1_ADDRH		0x00		//; E1的相对起始地址  高八位
#define busE1_ADDRL		0x82		//; E1的相对起始地址  低八位             0x  00   82      0x   00    83    7号消息
#define TS_ADDRH		0x00		//; TS的相对起始地址  高八位
#define TS_ADDRL		0x84		//; TS的相对起始地址  低八位             0x  00   84                       8号消息
#define DXC_ADDRH		0x00		//; 交叉的相对起始地址  高八位
#define DXC_ADDRL		0x84		//; 交叉的相对起始地址  低八位           0x  00   84
#define END_ADDRH		0x00		//; 所有信息的总个数
#define END_ADDRL		0x84		//;

//--------------------- Error FLAG -------------------------
#define Return_OK		   0x00		//; 没有错误

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

//业务类型
#define OP_NONE             0xFF          //无业务
#define OP_E1               0x01          //E1
#define OP_10BaseT		    0x02		  //10BaseT
#define OP_V35		        0x03		  //V.35
#define OP_RS232		    0x04 		  //RS232
#define OP_CHU		        0x05 		  //CHU,话路接口
#define OP_FXO		        0x06 		  //FXO,局端信令 SHL1218
#define OP_FXS		        0x07 		  //FXS,远端信令 SHL1318
#define OP_EM			    0x08 		  //E/M,E.M.信令
#define OP_CAR		        0x09 		  //载波卡
#define OP_RS485		    0x0A 		  //RS485
#define OP_Optical			0x0B		  //光接口
#define OP_MAC		        0x0C 		  //磁石卡


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

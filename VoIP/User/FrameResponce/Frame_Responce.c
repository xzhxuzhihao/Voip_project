#include "Frame_Responce.h"
#include "main.h"
#include "./usart/bsp_debug_usart.h"
//#include "SHL1318.h"
//#include "FPGA.h"
//#include "soft_spi.h"


extern volatile uint16_t USART_RX_STA,UsartSendFlag;
extern volatile uint8_t USART1_RX_Buff[MaxFrmLen];
extern volatile uint8_t Reg_rx[32];

extern volatile uint8_t MySlot;
//extern VoiceCircuit_typedef VoiceCircuit[32];
volatile uint8_t Card_Type,MSG_No,WRMode;
//volatile uint8_t DataAddrH,DataAddrL,DataLenH,DataLenL;

volatile uint8_t IsChange;
volatile uint8_t WarnAddrH,WarnAddrL,WarnLenH,WarnLenL;

volatile uint8_t MyState,busE1[2]; 

extern frame_typedef tx_frame,rx_frame;
Err_typedef Err;

static void PrepareTx(void);
static void WorkMsg1_IsChange(void);
static void WorkMsg1_STYLE(void);
static void WorkMsg1_STATE(void);

static void WorkMsg1_WARN(void);
static void MSG_WARN00(void);
static void MSG_WARN10(void);
static void MSG_WARN20(void);

static void WorkMsg1_CHAR(void);
static void WorkMsg1_MALM(void); 

static void WorkMsg1_PARA(void);
static void WritePara(void);
static void ReadPara(void);
static void MSG1_WPARA0000(void);
static void MSG1_WPARA0010(void);
static void MSG1_WPARA0020(void);
static void MSG1_WPARA0030(void);
static void MSG1_Read_Para0000(void);
static void MSG1_Read_Para0010(void);
static void MSG1_Read_Para0020(void);
static void MSG1_Read_Para0030(void);

static void WorkMsg1_busE1(void);
static void WorkMsg1_TS(void);
static void WorkMsg1_DXC(void);
static void WorkMsg1_Err(void);
static void WorkMsg1_RST(void);
static void WorkMsg1_OTHER(void);
static void TxVerify(void);
static void Read_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL);
static void Write_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL);
void Frame_Response(void)
{
    //设备内部消息结构()
    //帧头	帧长度 槽位号  盘类型	消息号	读写方式	起始地址	数据长度	  消息		帧尾
    // 2	  1	     1	    1	      1	       1		2		2	  00~10H	 1
	//FrmLength帧长度 //MOV	lenFrmRx1, #00H

	PrepareTx();//发送帧的帧头、帧长度、槽位号、盘类型、消息号、读写方式.
                                                                                 
	switch(rx_frame.MSG_No)                          
	{                                      
		case MSGA_IsChange:               
			WorkMsg1_IsChange();            //设备状态是否改变     0号消息
			break;                                                                     
		case MSGA_STYLE:                    //盘版本               1号消息
	        WorkMsg1_STYLE();
	        break;                                                                    
		case MSGA_STATE:                    //状态/结构  盘是否在机框里，如果盘在机框里，状态值为0x03;  2号消息
	         WorkMsg1_STATE();
	         break;                                                                       
		case MSGA_WARN:                     //告警               3号消息
	         WorkMsg1_WARN();
	         break;                         	                                           
		case MSGA_CHAR:                     //业务               4号消息
	         WorkMsg1_CHAR();               
	         break;                                                                     
		case MSGA_MALM:                     //告警屏蔽            5号消息
	         WorkMsg1_MALM();               
	         break;                                                                     
		case MSGA_PARA:                     //参数读写            6号消息
	         WorkMsg1_PARA();               
	         break;                         	                                            
		case MSGA_E1:                       //E1                  7号消息
	         WorkMsg1_busE1();
	         break;                         	                                            
		case MSGA_TS:                       //时隙                 8号消息
	         WorkMsg1_TS();
	         break;                         	                                             
		case MSGA_DXC:                      //交叉
	         WorkMsg1_DXC();
	         break;                        	                                          
		case MSGA_Err:                      //错误
	         WorkMsg1_Err();
	         break;                                                                       
		case MSGA_RST:                      //复位
	        WorkMsg1_RST();
	         break;                        
		default:                           
	         //WorkMsg1_OTHER();
	         break;                          
    }

}

//;==============================================================
//; PrepareTx : SET THE TX1 TO RECEIVE DEVICE
//; RETURN : R0:THE POINT TO THE DATA IN THE Tx_Buff
//; ATTENTION : the length of frame
//;--------------------------------------------------------------
static void PrepareTx(void)
{
    tx_frame.HeadH      = FrmHeadH_Up;
	tx_frame.HeadL      = FrmHeadL_Up;
	tx_frame.Length     = 0x1C;
	tx_frame.slot       = MySlot;
	tx_frame.card_type  = MyCardType;
	tx_frame.MSG_No     = rx_frame.MSG_No;
	tx_frame.WRMode     = rx_frame.WRMode; 
}
//==============================================================
// MSGA_IsChange消息的响应 0号消息
//--------------------------------------------------------------
static void WorkMsg1_IsChange(void)                  //设备状态是否变化
{
    tx_frame.StartAddrH   = 0x00;
    tx_frame.StartAddrL   = 0x00;
    tx_frame.DatLenH      = 0x00;	
	tx_frame.DatLenL      = 0x09;
	#if DEBUG
	    tx_frame.Data[0]  = 0xFF;
	#else
	    tx_frame.Data[0]  = IsChange; 
	#endif
	tx_frame.Data[1]  = MySlot;
	tx_frame.Data[2]  = MyCardType;
	tx_frame.Data[3]  = MyCardTypeVersion;
	tx_frame.Data[4]  = MSGA_WARN;
	tx_frame.Data[5]  = WarnAddrH;
	tx_frame.Data[6]  = WarnAddrL;
	tx_frame.Data[7]  = WarnLenH;
	tx_frame.Data[8]  = WarnLenL;
	tx_frame.Data[9]  = 0xFF;
	tx_frame.Data[10] = 0x00;
	tx_frame.Data[11] = 0x00;
	tx_frame.Data[12] = 0x00;
	tx_frame.Data[13] = 0x00;
	//tx_frame.Data[13] = 0x00;
	tx_frame.Data[14] = 0x00;
	tx_frame.Data[15] = 0x00;
    IsChange = NOCHANGE;
	WarnAddrH = 0x00;
	WarnAddrL = 0x00;
	WarnLenH  = 0x00;
	WarnLenL  = 0x00;
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
    Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1C为发送帧长度
}

//;==============================================================
//; MSGA_STYLE消息的响应  1号消息 不同消息的开始地址
//;--------------------------------------------------------------
static void WorkMsg1_STYLE(void)
{
    tx_frame.StartAddrH   = rx_frame.StartAddrH;
    tx_frame.StartAddrL   = rx_frame.StartAddrL;
    tx_frame.DatLenH      = rx_frame.DatLenH;	
	tx_frame.DatLenL      = rx_frame.DatLenL;
	
	tx_frame.Data[0]      = MyCardType;         
    tx_frame.Data[1]      = MyCardTypeVersion;  
	tx_frame.Data[2]      = SERVICE_ADDRH;      
	tx_frame.Data[3]      = SERVICE_ADDRL;      
	tx_frame.Data[4]      = MALM_ADDRH;         
	tx_frame.Data[5]      = MALM_ADDRL;        
	tx_frame.Data[6]      = PARA_ADDRH;         
	tx_frame.Data[7]      = PARA_ADDRL;         
	tx_frame.Data[8]      = busE1_ADDRH;           
	tx_frame.Data[9]      = busE1_ADDRL;        
	tx_frame.Data[10]     = TS_ADDRH;         
	tx_frame.Data[11]     = TS_ADDRL;          
	tx_frame.Data[12]     = DXC_ADDRH;          
	tx_frame.Data[13]     = DXC_ADDRL;         
	tx_frame.Data[14]     = END_ADDRH;          
	tx_frame.Data[15]     = END_ADDRL;
	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_STATE消息的响应,设备结构 2号消息  板卡是否在机框内
//;--------------------------------------------------------------
static void WorkMsg1_STATE(void)
{

    tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
    tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
    tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = 0x01;//rx_frame.DatLenL;//0x01

	MyState = 0x03;                             //0x03表示盘在机框中
	tx_frame.Data[0]      = MyState;         
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
    DEBUG_USART_TXD1_ACT_TOGGLE;		
}

//;==============================================================
//; MSGA_WARN消息的响应,告警状态 3号消息
//;--------------------------------------------------------------
static void WorkMsg1_WARN(void)
{
    tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
    tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00 0x10 0x20
    tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = rx_frame.DatLenL;       //0x10,0x10,0x10 	

    switch(rx_frame.StartAddrL)
	{
	    case 0x00:
			MSG_WARN00();
			break;
		case 0x10:
			MSG_WARN10();
			break;
		case 0x20:
			MSG_WARN20();
			break;
		default:
	        IsChange = CHANGED;                  
	        Err.ErrFlag  = Err_RX1ADDRESS;
		    Err.ErrMsg   = rx_frame.MSG_No;
		    Err.ErrAddrH = rx_frame.StartAddrH;
		    Err.ErrAddrL = rx_frame.StartAddrL;
		    Err.ErrLenH  = rx_frame.DatLenH;
		    Err.ErrLenL  = rx_frame.DatLenL;
		    //ErrFlag  = Err_RX1ADDRESS;     //                                          MOV	ErrFlag, #Err_RX1ADDRESS
	        //ErrMsg   = rx_frame.MSG_No;         //USART1_RX_Buff[5];    //MsgSi1;        串口接收的错误消息号             //MOV	ErrMsg, MsgSi1
	        //ErrAddrH = rx_frame.StartAddrH;     //USART1_RX_Buff[7];    //DAddrHSi1;     串口接收的错误消息地址高8位       //MOV	ErrAddrH, DAddrHSi1
	        //ErrAddrL = rx_frame.StartAddrL;     //USART1_RX_Buff[8];    //DAddrLSi1;     串口接收的错误消息地址低8位       //MOV	ErrAddrL, DAddrLSi1
	        //ErrLenH  = rx_frame.DatLenH;        //USART1_RX_Buff[9];    //DAllLenHSi1;   串口接收的错误消息长度高8位       //MOV	ErrLenH, DAllLenHSi1
	        //ErrLenL  = rx_frame.DatLenL;        //USART1_RX_Buff[10];   //DAllLenLSi1;   串口接收的错误消息长度低8位       //MOV	ErrLenL, DAllLenLSi1			
			break;	
	}

	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1C为发送帧长度 
    DEBUG_USART_TXD1_ACT_TOGGLE;		
}

//;==============================================================
//; MSGA_CHAR消息的响应  //业务  4号消息 32个话路的业务类型
//;--------------------------------------------------------------
static void WorkMsg1_CHAR(void)
{
	uint8_t len;

    tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
    tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00 0x10
    tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = rx_frame.DatLenL;       //0x10,0x10	

    for(len = 0x00; len < rx_frame.DatLenL; len++)    //tx_frame.StartAddrL = 0x00 or 0x10
	{
        tx_frame.Data[len] = 0x00;	//业务类型  根据情况填写
	}	

	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_MALM消息的响应, 告警屏蔽，实际没有告警屏蔽???  5号消息
//;--------------------------------------------------------------
static void WorkMsg1_MALM(void)
{
    if(rx_frame.WRMode == MODEWRITE)
    {
		tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
		tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00 0x10
		tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
		tx_frame.DatLenL      = 0x03;   //rx_frame.DatLenL;       //0x10,0x10
		
		tx_frame.Data[0]      = 0x06;
		tx_frame.Data[1]      = 0x06;
		tx_frame.Data[2]      = 0x06;				
    }
    else
    {
		tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
		tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
		tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
		tx_frame.DatLenL      = 0x00;          //rx_frame.DatLenL;       //0x10,0x10		   
   }
	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_PARA消息的响应,工作参数 6号消息
//;--------------------------------------------------------------
static void WorkMsg1_PARA(void)
{
    uint8_t i;

	
	if(rx_frame.WRMode == MODEWRITE)      //;0x02为WRITE,其余非0x02都看做是READ
    {
		tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
		tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00 0x10 0x20 0x30
		tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
		tx_frame.DatLenL      = rx_frame.DatLenL;       //0x10
		for(i = 0; i < tx_frame.DatLenL; i++)
		{
		    tx_frame.Data[i] = rx_frame.Data[i]; 
		}
		
		WritePara();
        IsChange = CHANGED;			
    }
    else
    {
        ReadPara();	   
    }	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}


static void WritePara(void)
{
    //uint16_t ParaAddr;
	
	//ParaAddr = rx_frame.StartAddrH;
	//ParaAddr = (ParaAddr << 0x08) + rx_frame.StartAddrL;
	switch(rx_frame.StartAddrL)
	{
		case 0x00:
			MSG1_WPARA0000();
		    IsChange = CHANGED;
			break;
		case 0x10:
			MSG1_WPARA0010();
		    IsChange = CHANGED;
			break;
		case 0x20:
			MSG1_WPARA0020();
		    IsChange = CHANGED;
			break;
		case 0x30:
			MSG1_WPARA0030();
		    IsChange = CHANGED;
			break;
		default:
	        IsChange = CHANGED;           
	        Err.ErrFlag  = Err_RX1ADDRESS;
		    Err.ErrMsg   = rx_frame.MSG_No;
		    Err.ErrAddrH = rx_frame.StartAddrH;
		    Err.ErrAddrL = rx_frame.StartAddrL;
		    Err.ErrLenH  = rx_frame.DatLenH;
		    Err.ErrLenL  = rx_frame.DatLenL;
			break;		
	}		
}

static void ReadPara(void)
{
    //uint16_t ParaAddr;
	
	//ParaAddr = rx_frame.StartAddrH;;
	//ParaAddr = (ParaAddr << 0x08) + rx_frame.StartAddrL;
    switch(rx_frame.StartAddrL)
	{
		case 0x00:
			MSG1_Read_Para0000();
			break;
		case 0x10:
			MSG1_Read_Para0010();
			break;
		case 0x20:
			MSG1_Read_Para0020();
			break;
		case 0x30:
			MSG1_Read_Para0030();
			break;
		default:
	        IsChange = CHANGED;        
	        Err.ErrFlag  = Err_RX1ADDRESS;
		    Err.ErrMsg   = rx_frame.MSG_No;
		    Err.ErrAddrH = rx_frame.StartAddrH;
		    Err.ErrAddrL = rx_frame.StartAddrL;
		    Err.ErrLenH  = rx_frame.DatLenH;
		    Err.ErrLenL  = rx_frame.DatLenL;
			break;		
	}	
}

//;==============================================================
//; MSGA_E1消息的响应,总线分配  7号消息
//;--------------------------------------------------------------

static void WorkMsg1_busE1(void)
{
    uint8_t i;

	tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
	tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
	tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = rx_frame.DatLenL;       //0x10  只有2个数据

	switch(rx_frame.WRMode)
	{
	    case MODEWRITE:
			for(i= 0x00; i < rx_frame.DatLenL; i++)
		    {
			    tx_frame.Data[i] = rx_frame.Data[i];
			}

			TxVerify();
			while(UsartSendFlag != UsartSendEnd){};
	        UsartSendFlag = UsartSendStart;
			Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);//0x1C为发送帧长度  	
            DEBUG_USART_TXD1_ACT_TOGGLE;
            Write_busE1(rx_frame.StartAddrH,rx_frame.StartAddrL);
			IsChange = CHANGED;
		    break;
		case MODEREAD:		    
		    Read_busE1(rx_frame.StartAddrH,rx_frame.StartAddrL);

			TxVerify();
			while(UsartSendFlag != UsartSendEnd){};
	        UsartSendFlag = UsartSendStart;
			Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
			DEBUG_USART_TXD1_ACT_TOGGLE;
			break;
		default:         //既不是MODEWRITE也不是MODEREAD
	        tx_frame.StartAddrH = rx_frame.StartAddrH;    
	        tx_frame.StartAddrL = rx_frame.StartAddrL;    
	        tx_frame.DatLenH    = rx_frame.DatLenH;        
	        tx_frame.DatLenL    = 0x02;//rx_frame.DatLenL; 

			TxVerify();
			while(UsartSendFlag != UsartSendEnd){};
	        UsartSendFlag = UsartSendStart;
			Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
            DEBUG_USART_TXD1_ACT_TOGGLE;				
		    break;
	}

}
//;==============================================================
//; MSGA_TS消息的响应,时隙分配  8号消息
//;--------------------------------------------------------------
static void WorkMsg1_TS(void)
{
 	   
	switch(rx_frame.WRMode)
	{
	    case MODEWRITE:
		
				tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
				tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00 
				tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
				tx_frame.DatLenL      = rx_frame.DatLenL;       //0x00

				TxVerify();
				while(UsartSendFlag != UsartSendEnd){};
				UsartSendFlag = UsartSendStart;
				Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1C为发送帧长度  	
	            DEBUG_USART_TXD1_ACT_TOGGLE;
			break;
		case MODEREAD:
		
				tx_frame.StartAddrH   = 0x00;                   //rx_frame.StartAddrH;    //0x00
				tx_frame.StartAddrL   = 0x00;                   //rx_frame.StartAddrL;    //0x00 0x10 0x20 0x30
				tx_frame.DatLenH      = 0x00;                   //rx_frame.DatLenH;	    //0x00
				tx_frame.DatLenL      = 0x00;                   //rx_frame.DatLenL;       //0x10
 	
			break;
		default:         //既不是MODEWRITE也不是MODEREAD
			
				tx_frame.StartAddrH   = 0x00;                   //rx_frame.StartAddrH;    //0x00
				tx_frame.StartAddrL   = 0x00;                   //rx_frame.StartAddrL;    //0x00 0x10 0x20 0x30
				tx_frame.DatLenH      = 0x00;                   //rx_frame.DatLenH;	    //0x00
				tx_frame.DatLenL      = 0x00;                   //rx_frame.DatLenL;       //0x10
	
		    break;
	}
    if(WRMode != MODEWRITE)
	{

			TxVerify();
			while(UsartSendFlag != UsartSendEnd){};
	        UsartSendFlag = UsartSendStart;
			Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
			DEBUG_USART_TXD1_ACT_TOGGLE;
	}

}
//;==============================================================
//; MSGA_DXC消息的响应,交叉配置  9号消息
//;--------------------------------------------------------------
static void WorkMsg1_DXC(void)
{

	tx_frame.StartAddrH   = 0x00;                   //rx_frame.StartAddrH;    //0x00
	tx_frame.StartAddrL   = 0x00;                   //rx_frame.StartAddrL;    //0x00 0x10 0x20 0x30
	tx_frame.DatLenH      = 0x00;                   //rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = 0x00;                   //rx_frame.DatLenL;       //0x10

	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_ERR消息的响应       10号消息
//;--------------------------------------------------------------
static void WorkMsg1_Err(void)
{

	tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
	tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
	tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = 0x09;                   //rx_frame.DatLenL;       //0x10
	
	tx_frame.Data[0]      = Err.ErrFlag;
	tx_frame.Data[1]      = MySlot;
	tx_frame.Data[2]      = MyCardType;
	tx_frame.Data[3]      = MyCardTypeVersion;
	tx_frame.Data[4]      = Err.ErrMsg;
	tx_frame.Data[5]      = Err.ErrAddrH;
	tx_frame.Data[6]      = Err.ErrAddrL;
	tx_frame.Data[7]      = Err.ErrLenH;
	tx_frame.Data[8]      = Err.ErrLenL;	

	         		
	Err.ErrFlag  = 0x00;        
	Err.ErrMsg   = 0x00;        
	Err.ErrAddrH = 0x00;       
	Err.ErrAddrL = 0x00;       
	Err.ErrLenH  = 0x00;       
	Err.ErrLenL  = 0x00;    
		
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); 
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_RST消息的响应   16号消息
//;--------------------------------------------------------------
static void WorkMsg1_RST(void)
{
  /* while(1)
   {	   
	    __disable_irq(); 
        __enable_irq();	   
   }*/

	TxVerify();
   	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}
	
/*static void WorkMsg1_OTHER(void)
{
    //uint8_t *p;
	uint8_t i,temp;
	switch(MSG_No)
	{
	    case 0x80:
			__disable_irq(); 
            i = 0x00;
		    temp = 0x00;
		    //temp = temp - 0x01;
            do
			{
			   Usart_SendFrame(&i,1);
			   i++;
		       temp = temp - 0x01;
		    }while(temp);
			__enable_irq(); 
			break;
		case 0x81:
			__disable_irq();
		    temp = 0x55;
		    Usart_SendFrame(&temp,1);
		    Usart_SendFrame(&temp,1);
		    //读取地址为ADDR_DXC_SoftVer的数据，地址减一再读取，一共读取32个数据

		    temp = 0x66;
		    Usart_SendFrame(&temp,1);
		    Usart_SendFrame(&temp,1);
		    
		
		    __enable_irq(); 
			break;
		case 0x82:
			__disable_irq();
			//READ_CP1122DXC
		    __enable_irq(); 
			break;
		case 0x83:
			__disable_irq();
			//Read ZL30122锟斤拷锟斤拷
		    __enable_irq(); 
			break;
		case 0x84:
			__disable_irq();
			//read1kRAM
		    __enable_irq(); 
			break;	
		case 0x85:
			__disable_irq();
			//READ_CP1122LOOP
		    __enable_irq(); 
			break;	
		case 0x86:
			__disable_irq();
			//REG_WR1122H,REG_WR1122L,WR_1122,RD_1122T 
		    __enable_irq(); 
			break;	
		case 0x87:
			__disable_irq();
			//RD_1122T
		    __enable_irq(); 
			break;	
		case 0x88:
			__disable_irq();
			//REG_Order Order_Change REG_A REG_K REG_L REG_M 
		    __enable_irq(); 
			break;	
		case 0x89:
			__disable_irq();
			//CONFIG_1122DXC
		    __enable_irq(); 
			break;	
		case 0x8A:
			__disable_irq();
			//锟斤拷锟斤拷锟斤拷DPTR锟斤拷锟斤拷锟斤拷锟斤拷
		    __enable_irq(); 
			break;
		case 0x8B:
			__disable_irq();
			//锟斤拷锟斤拷锟斤拷锟街节讹拷锟斤拷锟街斤拷写
		    __enable_irq(); 
			break;	
		default:
			//#Err_RX1MSG
            break;			
	}		

}
*/
static void TxVerify(void)
{
    uint8_t FrmLen,i,bit,result;
    uint8_t* p;
	
	result = 0;
	FrmLen = 0x1C;//USART1_TX_Buff[2];  //发送帧长度固定为0x1C,

	
	p = &tx_frame.HeadH;
	for(i = 0; i < (FrmLen-1); i++)  //帧尾
    {	
		for(bit = 0;bit < 8; bit++)
		{
            result = result + ((*p >> bit) & 0x01);
		}
		p++;
		result &= 0x01;	          //一个字节中1的个数是奇数还是偶数个?	
    }	

	result &= 0x01;               //bit0是否为0 
	if(result == 0x00) //
	{
	   tx_frame.Tail = 0x8D;                   
	}		
	else   
	{
	   tx_frame.Tail = 0x0D;                
	}		
}


static void MSG_WARN00(void)                        //1----16话路摘挂机状态上报
{
    uint8_t channel;


    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
	for(channel = 0x00; channel < 0x10; channel++)
	{
		/*if(VoiceCircuit[channel].ModuleType == OP_NONE)
			tx_frame.Data[channel] = 0x00;  //当没有电话时，同一上报0x00,挂机状态
		else if (VoiceCircuit[channel].ModuleType == OP_FXS)
		{
			if(VoiceCircuit[channel].tx_state == 0x01)		//只有0x00或0x01两状态，	分别为摘机，挂机,上报则为0x00：挂机,0x01摘机
				//VoiceCircuit[channel].tx_state = state;//挂机OnHook = 0x01; //摘机OnHook = 0x00;
				tx_frame.Data[channel] = 0x00;  //0x00为上报值，为挂机;//32个电话接口channel+1
			else
				tx_frame.Data[channel] = 0x0C;  //0x0C为上报值，为摘机;//32个电话接口channel+1		
		}
		else if (VoiceCircuit[channel].ModuleType == OP_FXO)
		{
		    if(VoiceCircuit[channel].tx_state == 0x01)       //摘机
			    tx_frame.Data[channel] = 0x0C;               //接收信令转换后的摘挂机状态
			else 
				tx_frame.Data[channel] = 0x00;
		}
		else
		{
		     ;//以后扩展
		}*/
	}

}
static void MSG_WARN10(void)                         //17----32话路摘挂机状态上报
{
    uint8_t channel;


    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
	for(channel = 0x00; channel < 0x10; channel++)
	{
		/*if(VoiceCircuit[channel + 0x10].ModuleType == OP_NONE)
			tx_frame.Data[channel] = 0x00;                   //当没有电话时，同一上报0x00,挂机状态
		else if (VoiceCircuit[channel + 0x10].ModuleType == OP_FXS)
		{
			if(VoiceCircuit[channel + 0x10].tx_state == 0x01)		//只有0x00或0x01两状态，	分别为摘机，挂机,上报则为0x00：挂机,0x01摘机
				tx_frame.Data[channel] = 0x00;               //0x00为上报值，为挂机;//32个电话接口channel+1
			else
				tx_frame.Data[channel] = 0x0C;              //0x0C为上报值，为摘机;//32个电话接口channel+1		
		}
		else if (VoiceCircuit[channel + 0x10].ModuleType == OP_FXO)
		{
		    if(VoiceCircuit[channel + 0x10].tx_state == 0x01)       //摘机
			    tx_frame.Data[channel] = 0x0C;               //接收信令转换后的摘挂机状态
			else 
				tx_frame.Data[channel] = 0x00;
		}
		else
		{
		     ;//以后扩展
		}*/
	}
                             
}
static void MSG_WARN20(void)
{
    //uint8_t channel;

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x20;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = 0x02;   //rx_frame.DatLenL;          //0x10;
	tx_frame.Data[0]    = 0x00;     //busE1 1 
	tx_frame.Data[1]    = 0x00;     //busE1 2	

}

static void MSG1_WPARA0000(void)
{
   volatile uint8_t  channel,tmpRegValue;
	
	//电话信令模式：1--16(高4位发送信令，低4位接收信令)，

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

    for(channel = 0x00; channel < 0x08; channel++)
	{
		/*tmpRegValue = (rx_frame.Data[channel] & 0xF0) + ((rx_frame.Data[channel + 0x08] & 0xF0) >> 0x04);
		FPGA_WriteReg(64 + channel,tmpRegValue);  //寄存器64-71：第1个E1发送信令
		tmpRegValue = ((rx_frame.Data[channel] & 0x0F) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x0F);
	    FPGA_WriteReg(80 + channel,tmpRegValue);  //寄存器80-87：第1个E1接收信令*/
	}

}

static void MSG1_WPARA0010(void)
{
    volatile uint8_t  channel,tmpRegValue;
	
	//电话信令模式：1--16(高4位发送信令，低4位接收信令)，

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
		
	//Startchannel = rx_frame.StartAddrL;                     //rx_frame.StartAddrL = 0x10;
	//Endchannel   = Startchannel + rx_frame.DatLenL;  //rx_frame.DatLenL = 0x10;
   /* for(channel = 0x00; channel < 0x08; channel++)
	{
	    //tmpRegValue = FPGA_ReadReg(64 + channel);
		tmpRegValue = (rx_frame.Data[channel] & 0xF0)               //高4位发送信令
		              + ((rx_frame.Data[channel + 0x08] & 0xF0) >> 0x04);//高4位发送信令
		FPGA_WriteReg(72 + channel,tmpRegValue);  //寄存器72-79：第2个E1发送信令
		tmpRegValue = ((rx_frame.Data[channel] & 0x0F) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x0F);
	    FPGA_WriteReg(88 + channel,tmpRegValue);  //寄存器88-95：第2个E1接收信令
	}*/
}
static void MSG1_WPARA0020(void)
{
    uint8_t  channel;//tmpRegValue;
	
	//电话配置(热线方式)：第1--16个

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x20;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	//Startchannel = 0x00;                                    //rx_frame.StartAddrL = 0x20;
	//Endchannel   = Startchannel + rx_frame.DatLenL;	        //rx_frame.DatLenL = 0x10;

	//寄存器96-103：第1个E1热线配置和呼叫状态  bit4  热线使能，高有效，时隙8-15
	                                       //bit0  热线使能，高有效，时隙24-31
	for(channel = 0x00; channel < 0x08; channel++)
	{
	   /* tmpRegValue = ((rx_frame.Data[channel] & 0x01) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x01);   //bit4 和 bit 0
		tmpRegValue += (FPGA_ReadReg(96 + channel) & 0xEE);  //寄存器96-103：第1个E1热线配置和呼叫状态
		FPGA_WriteReg(96 + channel,tmpRegValue);*/
	}

}

static void MSG1_WPARA0030(void)
{
   uint8_t  channel;//,tmpRegValue;
	
	//电话配置(热线方式)：第17--32个

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x30;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;


	//寄存器104-111：第2个E1热线配置和呼叫状态  bit4  热线使能，高有效，时隙8-15
	                                        //bit0  热线使能，高有效，时隙24-31
	for(channel = 0x00; channel < 0x08; channel++)
	{
	    /*tmpRegValue = ((rx_frame.Data[channel] & 0x01) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x01);   //bit4 和 bit 0
		tmpRegValue += (FPGA_ReadReg(104 + channel) & 0xEE);
		FPGA_WriteReg(104 + channel,tmpRegValue);*/
	}
}


static void MSG1_Read_Para0000(void)
{
    uint8_t  channel,Startchannel,Endchannel;
	
	//电话信令模式：1--16(高4位发送信令，低4位接收信令)，

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	Startchannel = tx_frame.StartAddrL;
	Endchannel   = Startchannel + rx_frame.DatLenL;
	for(channel = Startchannel; channel < Endchannel; channel++)                             //正常情况下，从0 ---- 15;
	{

        //tx_frame.Data[channel] = (VoiceCircuit[channel].tx_SignalingMode << 0x04) + VoiceCircuit[channel].rx_SignalingMode;		

	}
}

static void MSG1_Read_Para0010(void)
{
    uint8_t  channel,Startchannel,Endchannel;
	
	//电话信令模式：1--16(高4位发送信令，低4位接收信令)，

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	Startchannel = tx_frame.StartAddrL;
	Endchannel   = Startchannel + rx_frame.DatLenL;
	for(channel = Startchannel; channel < Endchannel; channel++)                             //正常情况下，从16 ---- 31;
	{
        //tx_frame.Data[channel - Startchannel] = (VoiceCircuit[channel].tx_SignalingMode << 0x04) + VoiceCircuit[channel].rx_SignalingMode;		
     
	}
}

static void MSG1_Read_Para0020(void)
{
   uint8_t  channel,Startchannel,Endchannel;
	
	//电话配置(热线方式)：第1--16个

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x20;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;


	Startchannel = 0x00;
	Endchannel   = Startchannel + rx_frame.DatLenL;
	
	for(channel = Startchannel; channel < Endchannel; channel++)                          
	{

		//tx_frame.Data[channel] = VoiceCircuit[channel].hotline;

	}
}

static void MSG1_Read_Para0030(void)
{
    uint8_t  channel,Startchannel,Endchannel;
	
	//电话配置(热线方式)：第17--32个

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x30;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	Startchannel = 0x10;
	Endchannel   = Startchannel + tx_frame.DatLenL;
	
	for(channel = Startchannel; channel < Endchannel; channel++)                          
	{

		//tx_frame.Data[channel - 0x10] = VoiceCircuit[channel].hotline;

	}
}

void Read_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL)    //总线配置E1 7号消息
{
    uint8_t len;
	
    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = 0x02;             //rx_frame.DatLenL;          //0x10;
 
    for(len = 0x00; len < tx_frame.DatLenL; len++)          //只有两个,len == 0和len == 1
	{

		tx_frame.Data[len] = busE1[E1bus_AddrL + len];

	}
}

void Write_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL) //总线配置E1 7号消息
{
    uint8_t len;
	

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = 0x02;             //rx_frame.DatLenL;          //0x10;
  
    for(len = 0x00; len < tx_frame.DatLenL; len++)      //只有两个,len == 0和len == 1
	{
	    //实际代码应该将接收数据写入相应寄存器中；
		//FPGA_WriteReg(len + 0x04, rx_frame.Data[len]);
		//busE1[E1bus_AddrL + len] = rx_frame.Data[len];//USART1_RX_Buff[E1bus_AddrL + len];
	}
}

void tx1_stateframe(void)
{
	uint8_t len;
	
    tx_frame.HeadH      = FrmHeadH_Up;
	tx_frame.HeadL      = FrmHeadL_Up;
	tx_frame.Length     = 0x1C;
	tx_frame.slot       = MySlot;
	tx_frame.card_type  = MyCardType;
	tx_frame.MSG_No     = 0x80;
	tx_frame.WRMode     = 0x01;
	
    tx_frame.StartAddrH = 0x00;       //0x00;
    tx_frame.StartAddrL = 0x00;       //0x30;
    tx_frame.DatLenH    = 0x00;         //0x00;
	tx_frame.DatLenL    = 0x10;          //0x10;
	for(len = 0x00; len<0x10;len++)
	{
	    tx_frame.Data[len] =  0x00;//数据还没有提供
	}	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);	
}

void tx2_stateframe(void)
{
	uint8_t len;
	
    tx_frame.HeadH      = FrmHeadH_Up;
	tx_frame.HeadL      = FrmHeadL_Up;
	tx_frame.Length     = 0x1C;
	tx_frame.slot       = MySlot;
	tx_frame.card_type  = MyCardType;
	tx_frame.MSG_No     = 0x80;
	tx_frame.WRMode     = 0x01;
		
    tx_frame.StartAddrH = 0x00;      
    tx_frame.StartAddrL = 0x10;       
    tx_frame.DatLenH    = 0x00;      
	tx_frame.DatLenL    = 0x10;        
	for(len = 0x10; len<0x20;len++)
	{
	    tx_frame.Data[len] =  0x00;//数据还没有提供
	}	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);	
}

void rx1_stateframe(void)
{
	uint8_t len;
	
    tx_frame.HeadH      = FrmHeadH_Up;
	tx_frame.HeadL      = FrmHeadL_Up;
	tx_frame.Length     = 0x1C;
	tx_frame.slot       = MySlot;
	tx_frame.card_type  = MyCardType;
	tx_frame.MSG_No     = 0x80;
	tx_frame.WRMode     = 0x01;

    tx_frame.StartAddrH = 0x00;       //0x00;
    tx_frame.StartAddrL = 0x20;       //0x30;
    tx_frame.DatLenH    = 0x00;         //0x00;
	tx_frame.DatLenL    = 0x10;          //0x10;
	for(len = 0x10; len<0x20;len++)
	{
	    tx_frame.Data[len] = 0x00;//数据还没有提供
	}	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
		
}

void rx2_stateframe(void)
{
	uint8_t len;
	
    tx_frame.HeadH      = FrmHeadH_Up;
	tx_frame.HeadL      = FrmHeadL_Up;
	tx_frame.Length     = 0x1C;
	tx_frame.slot       = MySlot;
	tx_frame.card_type  = MyCardType;
	tx_frame.MSG_No     = 0x80;
	tx_frame.WRMode     = 0x01;

    tx_frame.StartAddrH = 0x00;       //0x00;
    tx_frame.StartAddrL = 0x30;       //0x30;
    tx_frame.DatLenH    = 0x00;         //0x00;
	tx_frame.DatLenL    = 0x10;          //0x10;
	for(len = 0x10; len<0x20;len++)
	{
	    tx_frame.Data[len] = 0x00;//数据还没有提供
	}	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);		
}

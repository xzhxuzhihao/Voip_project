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
    //�豸�ڲ���Ϣ�ṹ()
    //֡ͷ	֡���� ��λ��  ������	��Ϣ��	��д��ʽ	��ʼ��ַ	���ݳ���	  ��Ϣ		֡β
    // 2	  1	     1	    1	      1	       1		2		2	  00~10H	 1
	//FrmLength֡���� //MOV	lenFrmRx1, #00H

	PrepareTx();//����֡��֡ͷ��֡���ȡ���λ�š������͡���Ϣ�š���д��ʽ.
                                                                                 
	switch(rx_frame.MSG_No)                          
	{                                      
		case MSGA_IsChange:               
			WorkMsg1_IsChange();            //�豸״̬�Ƿ�ı�     0����Ϣ
			break;                                                                     
		case MSGA_STYLE:                    //�̰汾               1����Ϣ
	        WorkMsg1_STYLE();
	        break;                                                                    
		case MSGA_STATE:                    //״̬/�ṹ  ���Ƿ��ڻ����������ڻ����״ֵ̬Ϊ0x03;  2����Ϣ
	         WorkMsg1_STATE();
	         break;                                                                       
		case MSGA_WARN:                     //�澯               3����Ϣ
	         WorkMsg1_WARN();
	         break;                         	                                           
		case MSGA_CHAR:                     //ҵ��               4����Ϣ
	         WorkMsg1_CHAR();               
	         break;                                                                     
		case MSGA_MALM:                     //�澯����            5����Ϣ
	         WorkMsg1_MALM();               
	         break;                                                                     
		case MSGA_PARA:                     //������д            6����Ϣ
	         WorkMsg1_PARA();               
	         break;                         	                                            
		case MSGA_E1:                       //E1                  7����Ϣ
	         WorkMsg1_busE1();
	         break;                         	                                            
		case MSGA_TS:                       //ʱ϶                 8����Ϣ
	         WorkMsg1_TS();
	         break;                         	                                             
		case MSGA_DXC:                      //����
	         WorkMsg1_DXC();
	         break;                        	                                          
		case MSGA_Err:                      //����
	         WorkMsg1_Err();
	         break;                                                                       
		case MSGA_RST:                      //��λ
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
// MSGA_IsChange��Ϣ����Ӧ 0����Ϣ
//--------------------------------------------------------------
static void WorkMsg1_IsChange(void)                  //�豸״̬�Ƿ�仯
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
    Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1CΪ����֡����
}

//;==============================================================
//; MSGA_STYLE��Ϣ����Ӧ  1����Ϣ ��ͬ��Ϣ�Ŀ�ʼ��ַ
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
//; MSGA_STATE��Ϣ����Ӧ,�豸�ṹ 2����Ϣ  �忨�Ƿ��ڻ�����
//;--------------------------------------------------------------
static void WorkMsg1_STATE(void)
{

    tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
    tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
    tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = 0x01;//rx_frame.DatLenL;//0x01

	MyState = 0x03;                             //0x03��ʾ���ڻ�����
	tx_frame.Data[0]      = MyState;         
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
    DEBUG_USART_TXD1_ACT_TOGGLE;		
}

//;==============================================================
//; MSGA_WARN��Ϣ����Ӧ,�澯״̬ 3����Ϣ
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
	        //ErrMsg   = rx_frame.MSG_No;         //USART1_RX_Buff[5];    //MsgSi1;        ���ڽ��յĴ�����Ϣ��             //MOV	ErrMsg, MsgSi1
	        //ErrAddrH = rx_frame.StartAddrH;     //USART1_RX_Buff[7];    //DAddrHSi1;     ���ڽ��յĴ�����Ϣ��ַ��8λ       //MOV	ErrAddrH, DAddrHSi1
	        //ErrAddrL = rx_frame.StartAddrL;     //USART1_RX_Buff[8];    //DAddrLSi1;     ���ڽ��յĴ�����Ϣ��ַ��8λ       //MOV	ErrAddrL, DAddrLSi1
	        //ErrLenH  = rx_frame.DatLenH;        //USART1_RX_Buff[9];    //DAllLenHSi1;   ���ڽ��յĴ�����Ϣ���ȸ�8λ       //MOV	ErrLenH, DAllLenHSi1
	        //ErrLenL  = rx_frame.DatLenL;        //USART1_RX_Buff[10];   //DAllLenLSi1;   ���ڽ��յĴ�����Ϣ���ȵ�8λ       //MOV	ErrLenL, DAllLenLSi1			
			break;	
	}

	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1CΪ����֡���� 
    DEBUG_USART_TXD1_ACT_TOGGLE;		
}

//;==============================================================
//; MSGA_CHAR��Ϣ����Ӧ  //ҵ��  4����Ϣ 32����·��ҵ������
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
        tx_frame.Data[len] = 0x00;	//ҵ������  ���������д
	}	

	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);
	DEBUG_USART_TXD1_ACT_TOGGLE;
}

//;==============================================================
//; MSGA_MALM��Ϣ����Ӧ, �澯���Σ�ʵ��û�и澯����???  5����Ϣ
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
//; MSGA_PARA��Ϣ����Ӧ,�������� 6����Ϣ
//;--------------------------------------------------------------
static void WorkMsg1_PARA(void)
{
    uint8_t i;

	
	if(rx_frame.WRMode == MODEWRITE)      //;0x02ΪWRITE,�����0x02��������READ
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
//; MSGA_E1��Ϣ����Ӧ,���߷���  7����Ϣ
//;--------------------------------------------------------------

static void WorkMsg1_busE1(void)
{
    uint8_t i;

	tx_frame.StartAddrH   = rx_frame.StartAddrH;    //0x00
	tx_frame.StartAddrL   = rx_frame.StartAddrL;    //0x00
	tx_frame.DatLenH      = rx_frame.DatLenH;	    //0x00
	tx_frame.DatLenL      = rx_frame.DatLenL;       //0x10  ֻ��2������

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
			Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);//0x1CΪ����֡����  	
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
		default:         //�Ȳ���MODEWRITEҲ����MODEREAD
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
//; MSGA_TS��Ϣ����Ӧ,ʱ϶����  8����Ϣ
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
				Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length); //0x1CΪ����֡����  	
	            DEBUG_USART_TXD1_ACT_TOGGLE;
			break;
		case MODEREAD:
		
				tx_frame.StartAddrH   = 0x00;                   //rx_frame.StartAddrH;    //0x00
				tx_frame.StartAddrL   = 0x00;                   //rx_frame.StartAddrL;    //0x00 0x10 0x20 0x30
				tx_frame.DatLenH      = 0x00;                   //rx_frame.DatLenH;	    //0x00
				tx_frame.DatLenL      = 0x00;                   //rx_frame.DatLenL;       //0x10
 	
			break;
		default:         //�Ȳ���MODEWRITEҲ����MODEREAD
			
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
//; MSGA_DXC��Ϣ����Ӧ,��������  9����Ϣ
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
//; MSGA_ERR��Ϣ����Ӧ       10����Ϣ
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
//; MSGA_RST��Ϣ����Ӧ   16����Ϣ
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
		    //��ȡ��ַΪADDR_DXC_SoftVer�����ݣ���ַ��һ�ٶ�ȡ��һ����ȡ32������

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
			//Read ZL30122����
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
			//������DPTR��������
		    __enable_irq(); 
			break;
		case 0x8B:
			__disable_irq();
			//�������ֽڶ����ֽ�д
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
	FrmLen = 0x1C;//USART1_TX_Buff[2];  //����֡���ȹ̶�Ϊ0x1C,

	
	p = &tx_frame.HeadH;
	for(i = 0; i < (FrmLen-1); i++)  //֡β
    {	
		for(bit = 0;bit < 8; bit++)
		{
            result = result + ((*p >> bit) & 0x01);
		}
		p++;
		result &= 0x01;	          //һ���ֽ���1�ĸ�������������ż����?	
    }	

	result &= 0x01;               //bit0�Ƿ�Ϊ0 
	if(result == 0x00) //
	{
	   tx_frame.Tail = 0x8D;                   
	}		
	else   
	{
	   tx_frame.Tail = 0x0D;                
	}		
}


static void MSG_WARN00(void)                        //1----16��·ժ�һ�״̬�ϱ�
{
    uint8_t channel;


    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
	for(channel = 0x00; channel < 0x10; channel++)
	{
		/*if(VoiceCircuit[channel].ModuleType == OP_NONE)
			tx_frame.Data[channel] = 0x00;  //��û�е绰ʱ��ͬһ�ϱ�0x00,�һ�״̬
		else if (VoiceCircuit[channel].ModuleType == OP_FXS)
		{
			if(VoiceCircuit[channel].tx_state == 0x01)		//ֻ��0x00��0x01��״̬��	�ֱ�Ϊժ�����һ�,�ϱ���Ϊ0x00���һ�,0x01ժ��
				//VoiceCircuit[channel].tx_state = state;//�һ�OnHook = 0x01; //ժ��OnHook = 0x00;
				tx_frame.Data[channel] = 0x00;  //0x00Ϊ�ϱ�ֵ��Ϊ�һ�;//32���绰�ӿ�channel+1
			else
				tx_frame.Data[channel] = 0x0C;  //0x0CΪ�ϱ�ֵ��Ϊժ��;//32���绰�ӿ�channel+1		
		}
		else if (VoiceCircuit[channel].ModuleType == OP_FXO)
		{
		    if(VoiceCircuit[channel].tx_state == 0x01)       //ժ��
			    tx_frame.Data[channel] = 0x0C;               //��������ת�����ժ�һ�״̬
			else 
				tx_frame.Data[channel] = 0x00;
		}
		else
		{
		     ;//�Ժ���չ
		}*/
	}

}
static void MSG_WARN10(void)                         //17----32��·ժ�һ�״̬�ϱ�
{
    uint8_t channel;


    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
	for(channel = 0x00; channel < 0x10; channel++)
	{
		/*if(VoiceCircuit[channel + 0x10].ModuleType == OP_NONE)
			tx_frame.Data[channel] = 0x00;                   //��û�е绰ʱ��ͬһ�ϱ�0x00,�һ�״̬
		else if (VoiceCircuit[channel + 0x10].ModuleType == OP_FXS)
		{
			if(VoiceCircuit[channel + 0x10].tx_state == 0x01)		//ֻ��0x00��0x01��״̬��	�ֱ�Ϊժ�����һ�,�ϱ���Ϊ0x00���һ�,0x01ժ��
				tx_frame.Data[channel] = 0x00;               //0x00Ϊ�ϱ�ֵ��Ϊ�һ�;//32���绰�ӿ�channel+1
			else
				tx_frame.Data[channel] = 0x0C;              //0x0CΪ�ϱ�ֵ��Ϊժ��;//32���绰�ӿ�channel+1		
		}
		else if (VoiceCircuit[channel + 0x10].ModuleType == OP_FXO)
		{
		    if(VoiceCircuit[channel + 0x10].tx_state == 0x01)       //ժ��
			    tx_frame.Data[channel] = 0x0C;               //��������ת�����ժ�һ�״̬
			else 
				tx_frame.Data[channel] = 0x00;
		}
		else
		{
		     ;//�Ժ���չ
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
	
	//�绰����ģʽ��1--16(��4λ���������4λ��������)��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

    for(channel = 0x00; channel < 0x08; channel++)
	{
		/*tmpRegValue = (rx_frame.Data[channel] & 0xF0) + ((rx_frame.Data[channel + 0x08] & 0xF0) >> 0x04);
		FPGA_WriteReg(64 + channel,tmpRegValue);  //�Ĵ���64-71����1��E1��������
		tmpRegValue = ((rx_frame.Data[channel] & 0x0F) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x0F);
	    FPGA_WriteReg(80 + channel,tmpRegValue);  //�Ĵ���80-87����1��E1��������*/
	}

}

static void MSG1_WPARA0010(void)
{
    volatile uint8_t  channel,tmpRegValue;
	
	//�绰����ģʽ��1--16(��4λ���������4λ��������)��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;
		
	//Startchannel = rx_frame.StartAddrL;                     //rx_frame.StartAddrL = 0x10;
	//Endchannel   = Startchannel + rx_frame.DatLenL;  //rx_frame.DatLenL = 0x10;
   /* for(channel = 0x00; channel < 0x08; channel++)
	{
	    //tmpRegValue = FPGA_ReadReg(64 + channel);
		tmpRegValue = (rx_frame.Data[channel] & 0xF0)               //��4λ��������
		              + ((rx_frame.Data[channel + 0x08] & 0xF0) >> 0x04);//��4λ��������
		FPGA_WriteReg(72 + channel,tmpRegValue);  //�Ĵ���72-79����2��E1��������
		tmpRegValue = ((rx_frame.Data[channel] & 0x0F) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x0F);
	    FPGA_WriteReg(88 + channel,tmpRegValue);  //�Ĵ���88-95����2��E1��������
	}*/
}
static void MSG1_WPARA0020(void)
{
    uint8_t  channel;//tmpRegValue;
	
	//�绰����(���߷�ʽ)����1--16��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x20;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	//Startchannel = 0x00;                                    //rx_frame.StartAddrL = 0x20;
	//Endchannel   = Startchannel + rx_frame.DatLenL;	        //rx_frame.DatLenL = 0x10;

	//�Ĵ���96-103����1��E1�������úͺ���״̬  bit4  ����ʹ�ܣ�����Ч��ʱ϶8-15
	                                       //bit0  ����ʹ�ܣ�����Ч��ʱ϶24-31
	for(channel = 0x00; channel < 0x08; channel++)
	{
	   /* tmpRegValue = ((rx_frame.Data[channel] & 0x01) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x01);   //bit4 �� bit 0
		tmpRegValue += (FPGA_ReadReg(96 + channel) & 0xEE);  //�Ĵ���96-103����1��E1�������úͺ���״̬
		FPGA_WriteReg(96 + channel,tmpRegValue);*/
	}

}

static void MSG1_WPARA0030(void)
{
   uint8_t  channel;//,tmpRegValue;
	
	//�绰����(���߷�ʽ)����17--32��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x30;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;


	//�Ĵ���104-111����2��E1�������úͺ���״̬  bit4  ����ʹ�ܣ�����Ч��ʱ϶8-15
	                                        //bit0  ����ʹ�ܣ�����Ч��ʱ϶24-31
	for(channel = 0x00; channel < 0x08; channel++)
	{
	    /*tmpRegValue = ((rx_frame.Data[channel] & 0x01) << 0x04) + (rx_frame.Data[channel + 0x08] & 0x01);   //bit4 �� bit 0
		tmpRegValue += (FPGA_ReadReg(104 + channel) & 0xEE);
		FPGA_WriteReg(104 + channel,tmpRegValue);*/
	}
}


static void MSG1_Read_Para0000(void)
{
    uint8_t  channel,Startchannel,Endchannel;
	
	//�绰����ģʽ��1--16(��4λ���������4λ��������)��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	Startchannel = tx_frame.StartAddrL;
	Endchannel   = Startchannel + rx_frame.DatLenL;
	for(channel = Startchannel; channel < Endchannel; channel++)                             //��������£���0 ---- 15;
	{

        //tx_frame.Data[channel] = (VoiceCircuit[channel].tx_SignalingMode << 0x04) + VoiceCircuit[channel].rx_SignalingMode;		

	}
}

static void MSG1_Read_Para0010(void)
{
    uint8_t  channel,Startchannel,Endchannel;
	
	//�绰����ģʽ��1--16(��4λ���������4λ��������)��

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x10;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = rx_frame.DatLenL;          //0x10;

	Startchannel = tx_frame.StartAddrL;
	Endchannel   = Startchannel + rx_frame.DatLenL;
	for(channel = Startchannel; channel < Endchannel; channel++)                             //��������£���16 ---- 31;
	{
        //tx_frame.Data[channel - Startchannel] = (VoiceCircuit[channel].tx_SignalingMode << 0x04) + VoiceCircuit[channel].rx_SignalingMode;		
     
	}
}

static void MSG1_Read_Para0020(void)
{
   uint8_t  channel,Startchannel,Endchannel;
	
	//�绰����(���߷�ʽ)����1--16��

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
	
	//�绰����(���߷�ʽ)����17--32��

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

void Read_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL)    //��������E1 7����Ϣ
{
    uint8_t len;
	
    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = 0x02;             //rx_frame.DatLenL;          //0x10;
 
    for(len = 0x00; len < tx_frame.DatLenL; len++)          //ֻ������,len == 0��len == 1
	{

		tx_frame.Data[len] = busE1[E1bus_AddrL + len];

	}
}

void Write_busE1(uint8_t E1bus_AddrH,uint8_t E1bus_AddrL) //��������E1 7����Ϣ
{
    uint8_t len;
	

    tx_frame.StartAddrH = rx_frame.StartAddrH;       //0x00;
    tx_frame.StartAddrL = rx_frame.StartAddrL;       //0x00;
    tx_frame.DatLenH    = rx_frame.DatLenH;          //0x00;
	tx_frame.DatLenL    = 0x02;             //rx_frame.DatLenL;          //0x10;
  
    for(len = 0x00; len < tx_frame.DatLenL; len++)      //ֻ������,len == 0��len == 1
	{
	    //ʵ�ʴ���Ӧ�ý���������д����Ӧ�Ĵ����У�
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
	    tx_frame.Data[len] =  0x00;//���ݻ�û���ṩ
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
	    tx_frame.Data[len] =  0x00;//���ݻ�û���ṩ
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
	    tx_frame.Data[len] = 0x00;//���ݻ�û���ṩ
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
	    tx_frame.Data[len] = 0x00;//���ݻ�û���ṩ
	}	
	TxVerify();
	while(UsartSendFlag != UsartSendEnd){};
	UsartSendFlag = UsartSendStart;
	Usart_SendFrame_IT(&tx_frame.HeadH,tx_frame.Length);		
}

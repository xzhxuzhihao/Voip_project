/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS V9.0.0 + STM32 LwIP
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include "main.h"
#include "gpio.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "udpecho.h"
//#include "w25qxx.h"
#include "eXosip2.h"
//#include <osip2/internal.h>
#include <osip2/osip.h>
#include <osipparser2/osip_parser.h>
#include <osipparser2/sdp_message.h>


/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Test1_Task_Handle = NULL;/* LED������ */
static TaskHandle_t Test2_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t Board_Hardware_Check_Task_Handle = NULL;/* KEY������ */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
*/
EventGroupHandle_t EventGroupHandler;       /* �¼���־���� */




/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
extern uint8_t  Frame_send_type;

/******************************* �궨�� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩ�궨�塣
 */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void Test1_Task(void* pvParameters);/* Test1_Task����ʵ�� */
static void Test2_Task(void* pvParameters);/* Test2_Task����ʵ�� */
extern void Board_Hardware_Check_Task(void* pvParameters); /*�Ȳ��*/
extern void TCPIP_Init(void);

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/

int main(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  BSP_Init();

	uint8_t str_test[10];
	str_test[0] = '1';
	str_test[1] = '2';
	str_test[2] = '3';
	str_test[3] = '\n';
	str_test[4] = '\0';
	Usart_SendString(str_test);
	

  /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  TCPIP_Init();
	
  udpecho_init();
	
	uint8_t str_test[10];
	str_test[0] = '1';
	str_test[1] = '2';
	str_test[2] = '4';
	str_test[3] = '\n';
	str_test[4] = '\0';
	Usart_SendString(str_test);
  
  printf("��������ʾTCP����ʵ��,���Է������ݵ�������,�����彫���ݷ��ص�������\r\n");
  
  printf("��������ģ�����£�\n\t ����<--����-->·��<--����-->������\r\n");
  
  printf("ʵ����ʹ��TCP Э�鴫�����ݣ�������ΪTCP Client����������Ϊ TCP Server\r\n");
  
  printf("�����̵�IP��ַ����User/arch/sys_arch.h�ļ����޸�\r\n");
    
  printf("�����̲ο�<<LwIPӦ��ʵս����ָ��>> ʹ�� NETCONN �ӿڱ��\r\n");
  
  printf("�뽫������λ������ΪTCP Client.���뿪�����IP��ַ(��192.168.0.11)\r\n");  
  
  printf("�����޸�IP��ַ��˿ںţ����޸Ķ�Ӧ�ĺ궨��:IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3,LOCAL_PORT\r\n");  
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* �����¼���־�� */
 //   EventGroupHandler = xEventGroupCreate();
 	int port = 8407;
	int optval;
	int automasquerade = 0;
	int expiry = 3600;
	const char *proxy = "sip:192.168.0.144";
	const char *from = "sip:1002@192.168.0.144";
	const char *contact = "<sip:1002@192.168.0.11:8407;transport=udp>;expires=3600";
	const char *username = "1002";
	const char *password = "1234";
	const char *prog_name = "eXosip2-5.3.0";
	int wait_mtime = 100;
	eXosip_event_t *event;
	 struct eXosip_t *ptr_t = (struct eXosip_t *) osip_malloc(sizeof(eXosip_t)); //osip_malloc is malloc
		printf("sizeof(eXosip_t) = %d\n",sizeof(eXosip_t)); // 55k
    struct eXosip_t *context = eXosip_malloc();
//    osip_trace_initialize_func(OSIP_INFO4, &my_trace_func);
    if (eXosip_init(context) != 0) {
        printf("Couldn't initialize eXosip!\n");
        return ;
    }

// create a thread and build a udp socket and excontext->j_thread stores the thread pointer
		// the thread is using to power the state machine
    if (eXosip_listen_addr(context, IPPROTO_UDP, NULL, port, AF_INET, 0) != 0) {
        printf("Couldn't initialize transport layer!\n");
        eXosip_quit(context);
        return ;
    }
		str_test[0] = '1';
		str_test[1] = '2';
		str_test[2] = '5';
		str_test[3] = '\n';
		str_test[4] = '\0';
		Usart_SendString(str_test);
		
	optval = automasquerade;
	eXosip_set_option(context, EXOSIP_OPT_AUTO_MASQUERADE_CONTACT, &optval);
	str_test[0] = '1';
	str_test[1] = '2';
	str_test[2] = '6';
	str_test[3] = '\n';
	str_test[4] = '\0';
	Usart_SendString(str_test);
	eXosip_set_user_agent(context, prog_name);
	eXosip_add_authentication_info(context, username, username, password, NULL, NULL);

    // Build the REGISTER request
	{
		osip_message_t *reg = NULL;
		int result_judge = eXosip_register_build_initial_register(context, from, proxy, contact, expiry*2, &reg);
		if(result_judge<1) printf("error.\n");
											   
		// Send the REGISTER request
		int result_judge_1 = eXosip_register_send_register(context, result_judge, reg);
		if(result_judge_1!=0) printf("error.\n");
	}

	while ((event = eXosip_event_wait(context, 0, wait_mtime)) != NULL) {
		eXosip_automatic_action(context);
		switch (event->type) {
			case EXOSIP_REGISTRATION_SUCCESS:
				printf("registered successfully\n");
				break;
			case EXOSIP_REGISTRATION_FAILURE:
				printf("registered failed.\n");
				break;
			case EXOSIP_MESSAGE_NEW:{
				osip_message_t *answer;
				int i= eXosip_message_build_answer(context, event->tid, 405, &answer);
				if (i != 0){
					printf("failed to reject %s\n", event->request->sip_method);
					break;
				}
				i = eXosip_message_send_answer(context, event->tid, 405, answer);

				if (i != 0) {
					printf("failed to reject %s\n", event->request->sip_method);
					break;
				}

				printf("%s rejected with 405", event->request->sip_method);
				break;
			}
			case EXOSIP_CALL_INVITE: {
				osip_message_t *answer;
				int i;

				i = eXosip_call_build_answer(context, event->tid, 405, &answer);

				if (i != 0) {
				  printf("failed to reject %s\n", event->request->sip_method);
				  break;
				}
				osip_free(answer->reason_phrase);
				answer->reason_phrase = osip_strdup("No Support for Incoming Calls");
				i = eXosip_call_send_answer(context, event->tid, 405, answer);

				if (i != 0) {
				  printf("failed to reject %s\n", event->request->sip_method);
				  break;
				}
			
				printf("%s rejected with 405", event->request->sip_method);
				break;
			}
			case EXOSIP_CALL_CLOSED:
			case EXOSIP_CALL_RELEASED:
				break;
			default:
				printf("others message\n");
				break;
		}
		eXosip_event_free(event);
	}

    eXosip_quit(context);
	osip_free(context);
   taskEXIT_CRITICAL();            //�˳��ٽ���
		while(1) ;
 // ����Test1_Task���� 
 // xReturn = xTaskCreate((TaskFunction_t )Test1_Task, /* ������ں��� */
 //                       (const char*    )"Test1_Task",/* �������� */
 //                       (uint16_t       )256,   /* ����ջ��С */
 //                       (void*          )NULL,	/* ������ں������� */
 //                       (UBaseType_t    )1,	    /* ��������ȼ� */
//                        (TaskHandle_t*  )&Test1_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("Create Test1_Task sucess...\r\n");
//  
//  /* ����Test2_Task���� */
//  xReturn = xTaskCreate((TaskFunction_t )Test2_Task,  /* ������ں��� */
//                        (const char*    )"Test2_Task",/* �������� */
//                        (uint16_t       )256,  /* ����ջ��С */
//                        (void*          )NULL,/* ������ں������� */
//                        (UBaseType_t    )2, /* ��������ȼ� */
//                        (TaskHandle_t*  )&Test2_Task_Handle);/* ������ƿ�ָ�� */ 
//  if(pdPASS == xReturn)
//    printf("Create Test2_Task sucess...\r\n");

//  /* ����Board_Hardware_Check__Task���� */
//  xReturn = xTaskCreate((TaskFunction_t )Board_Hardware_Check_Task,  /* ������ں��� */
//                        (const char*    )"Board_Hardware_Check__Task",/* �������� */
//                        (uint16_t       )256,  /* ����ջ��С */
//                        (void*          )NULL,/* ������ں������� */
//                        (UBaseType_t    )5, /* ��������ȼ� */
//                        (TaskHandle_t*  )&Board_Hardware_Check_Task_Handle);/* ������ƿ�ָ�� */   
//  if(pdPASS == xReturn)
//    printf("Create Board_Hardware_Check__Task sucess...\r\n");
//  else
//		printf("Create Board_Hardware_Check__Task failed...\r\n");

//  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
//  
//  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� Test1_Task
  * @ ����˵���� Test1_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
/*
extern uint8_t slot;
static void Test1_Task(void* parameter)
{	
  uint8_t i=0;
  while (1)
  {
       //if(i==1){
		taskENTER_CRITICAL();  //�����ٽ��� 
	  //FPGA_Read_TotalReg_DMA(slot,76);//��ʱ��������ɴ˹�������Ϊ���԰��ʵ�ʰ����Ӳ���в��
	  //i = 0;
	  //printf("slot=%d,spiRcvBuff[3+4*%d]=%d\r\n",slot,i,spiRcvBuff[3+4*i]);
	  //for(i=0;i<19;i++)
	  //{
	   //printf("slot=%d,spiRcvBuff[3+4*%d]=%d\r\n",slot,i,spiRcvBuff[3+4*i]);
	  //}
	 		  
	  	FPGA_ReadReg_DMA(i,GeneralRegBaseAddr,Reg00); //FPGA_ReadReg(slot,0);
	    printf("slot=%d,spiRcvBuff[3]=%d\r\n",i,spiRcvBuff[3]);		   
	  taskEXIT_CRITICAL();	//�˳��ٽ���
	  // }
	  if(i<5)
		  i++;
	  else
		  i=0;
	  printf("RUN1_LED TASK IS RUNNING\r\n\r\n");
	  RUN1_LED = !RUN1_LED;//LED1_TOGGLE;
//    PRINT_DEBUG("LED1_TOGGLE\n");
      vTaskDelay(500);// ��ʱ500��tick 
  }
}
*/
/**********************************************************************
  * @ ������  �� Test2_Task
  * @ ����˵���� Test2_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
/*
extern ETH_HandleTypeDef heth;  
static void Test2_Task(void* parameter)
{	 
  uint16_t type;
  uint32_t linkstatus;
	while (1)
  {
       taskENTER_CRITICAL();  //�����ٽ���     
	  Frame_send_type = HeartBeat_Frame;				
	  //LWIP_Status |= LWIP_SEND_DATA;
	  LWIP_Status |= LWIP_SEND_DATA;
	  type = W25QXX_ReadID();
	  //for(i=0;i<19;i++)
	  //{
      //if(type != 0xEF12)
	      printf("W25QXX_ReadID = %hX\r\n",type);
	  //}
	 // if(slot<15)
	//	  slot++;
	//  else
	//	  slot=0;  
      linkstatus = LAN8720_Get_PHY_LINK_STATUS();
       printf("linkstatus = %X\r\n",linkstatus);	  
      taskEXIT_CRITICAL();	//�˳��ٽ���  
	  //printf("Heartbeat TASK IS RUNNING\r\n\r\n");
             	  
	  TP4 = !TP4;  //LED2_TOGGLE;
	//TP5 = !TP5;  //����
//    PRINT_DEBUG("LED2_TOGGLE\n");
	//printf("���������ݷ���\r\n");
    vTaskDelay(500);// ��ʱ100��tick 
  }
}
*/
/**********************************************************************
  * @ ������  �� Test2_Task
  * @ ����˵���� Test2_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
//����
/*
void heartbeat_task(void *pdata)
{
//	OS_CPU_SR cpu_sr=0;

	while(1)
	{
		
		Frame_send_type = HeartBeat_Frame;				
		LWIP_Status |= LWIP_SEND_DATA;
		printf("\r\nHeartbeat TASK IS RUNNING\r\n");
		//OS_ENTER_CRITICAL();	//�����ٽ���(�ر��ж�)
		//printf("����\r\n"); //���ڴ�ӡ���
		//OS_EXIT_CRITICAL();		//�˳��ٽ���(���ж�)
		OSTimeDlyHMSM(0,0,2,0);
	}

}
*/

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
/********************************END OF FILE****************************/

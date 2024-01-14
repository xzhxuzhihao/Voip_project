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
  * 实验平台:野火  STM32全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
#include "main.h"
#include "gpio.h"
/* FreeRTOS头文件 */
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


/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t Test1_Task_Handle = NULL;/* LED任务句柄 */
static TaskHandle_t Test2_Task_Handle = NULL;/* KEY任务句柄 */
static TaskHandle_t Board_Hardware_Check_Task_Handle = NULL;/* KEY任务句柄 */

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
*/
EventGroupHandle_t EventGroupHandler;       /* 事件标志组句柄 */




/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
extern uint8_t  Frame_send_type;

/******************************* 宏定义 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些宏定义。
 */


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void Test1_Task(void* pvParameters);/* Test1_Task任务实现 */
static void Test2_Task(void* pvParameters);/* Test2_Task任务实现 */
extern void Board_Hardware_Check_Task(void* pvParameters); /*热插拔*/
extern void TCPIP_Init(void);

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/

int main(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  /* 开发板硬件初始化 */
  BSP_Init();

	uint8_t str_test[10];
	str_test[0] = '1';
	str_test[1] = '2';
	str_test[2] = '3';
	str_test[3] = '\n';
	str_test[4] = '\0';
	Usart_SendString(str_test);
	

  /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  TCPIP_Init();
	
  udpecho_init();
	
	uint8_t str_test[10];
	str_test[0] = '1';
	str_test[1] = '2';
	str_test[2] = '4';
	str_test[3] = '\n';
	str_test[4] = '\0';
	Usart_SendString(str_test);
  
  printf("本例程演示TCP回显实验,电脑发送数据到开发板,开发板将数据返回到电脑上\r\n");
  
  printf("网络连接模型如下：\n\t 电脑<--网线-->路由<--网线-->开发板\r\n");
  
  printf("实验中使用TCP 协议传输数据，电脑作为TCP Client，开发板作为 TCP Server\r\n");
  
  printf("本例程的IP地址均在User/arch/sys_arch.h文件中修改\r\n");
    
  printf("本例程参考<<LwIP应用实战开发指南>> 使用 NETCONN 接口编程\r\n");
  
  printf("请将电脑上位机设置为TCP Client.输入开发板的IP地址(如192.168.0.11)\r\n");  
  
  printf("如需修改IP地址与端口号，则修改对应的宏定义:IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3,LOCAL_PORT\r\n");  
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建事件标志组 */
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
   taskEXIT_CRITICAL();            //退出临界区
		while(1) ;
 // 创建Test1_Task任务 
 // xReturn = xTaskCreate((TaskFunction_t )Test1_Task, /* 任务入口函数 */
 //                       (const char*    )"Test1_Task",/* 任务名字 */
 //                       (uint16_t       )256,   /* 任务栈大小 */
 //                       (void*          )NULL,	/* 任务入口函数参数 */
 //                       (UBaseType_t    )1,	    /* 任务的优先级 */
//                        (TaskHandle_t*  )&Test1_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("Create Test1_Task sucess...\r\n");
//  
//  /* 创建Test2_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )Test2_Task,  /* 任务入口函数 */
//                        (const char*    )"Test2_Task",/* 任务名字 */
//                        (uint16_t       )256,  /* 任务栈大小 */
//                        (void*          )NULL,/* 任务入口函数参数 */
//                        (UBaseType_t    )2, /* 任务的优先级 */
//                        (TaskHandle_t*  )&Test2_Task_Handle);/* 任务控制块指针 */ 
//  if(pdPASS == xReturn)
//    printf("Create Test2_Task sucess...\r\n");

//  /* 创建Board_Hardware_Check__Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )Board_Hardware_Check_Task,  /* 任务入口函数 */
//                        (const char*    )"Board_Hardware_Check__Task",/* 任务名字 */
//                        (uint16_t       )256,  /* 任务栈大小 */
//                        (void*          )NULL,/* 任务入口函数参数 */
//                        (UBaseType_t    )5, /* 任务的优先级 */
//                        (TaskHandle_t*  )&Board_Hardware_Check_Task_Handle);/* 任务控制块指针 */   
//  if(pdPASS == xReturn)
//    printf("Create Board_Hardware_Check__Task sucess...\r\n");
//  else
//		printf("Create Board_Hardware_Check__Task failed...\r\n");

//  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
//  
//  taskEXIT_CRITICAL();            //退出临界区
}



/**********************************************************************
  * @ 函数名  ： Test1_Task
  * @ 功能说明： Test1_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
/*
extern uint8_t slot;
static void Test1_Task(void* parameter)
{	
  uint8_t i=0;
  while (1)
  {
       //if(i==1){
		taskENTER_CRITICAL();  //进入临界区 
	  //FPGA_Read_TotalReg_DMA(slot,76);//暂时还不能完成此工作，因为调试板和实际板相关硬件有差别
	  //i = 0;
	  //printf("slot=%d,spiRcvBuff[3+4*%d]=%d\r\n",slot,i,spiRcvBuff[3+4*i]);
	  //for(i=0;i<19;i++)
	  //{
	   //printf("slot=%d,spiRcvBuff[3+4*%d]=%d\r\n",slot,i,spiRcvBuff[3+4*i]);
	  //}
	 		  
	  	FPGA_ReadReg_DMA(i,GeneralRegBaseAddr,Reg00); //FPGA_ReadReg(slot,0);
	    printf("slot=%d,spiRcvBuff[3]=%d\r\n",i,spiRcvBuff[3]);		   
	  taskEXIT_CRITICAL();	//退出临界区
	  // }
	  if(i<5)
		  i++;
	  else
		  i=0;
	  printf("RUN1_LED TASK IS RUNNING\r\n\r\n");
	  RUN1_LED = !RUN1_LED;//LED1_TOGGLE;
//    PRINT_DEBUG("LED1_TOGGLE\n");
      vTaskDelay(500);// 延时500个tick 
  }
}
*/
/**********************************************************************
  * @ 函数名  ： Test2_Task
  * @ 功能说明： Test2_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
/*
extern ETH_HandleTypeDef heth;  
static void Test2_Task(void* parameter)
{	 
  uint16_t type;
  uint32_t linkstatus;
	while (1)
  {
       taskENTER_CRITICAL();  //进入临界区     
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
      taskEXIT_CRITICAL();	//退出临界区  
	  //printf("Heartbeat TASK IS RUNNING\r\n\r\n");
             	  
	  TP4 = !TP4;  //LED2_TOGGLE;
	//TP5 = !TP5;  //测试
//    PRINT_DEBUG("LED2_TOGGLE\n");
	//printf("设置有数据发送\r\n");
    vTaskDelay(500);// 延时100个tick 
  }
}
*/
/**********************************************************************
  * @ 函数名  ： Test2_Task
  * @ 功能说明： Test2_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
//心跳
/*
void heartbeat_task(void *pdata)
{
//	OS_CPU_SR cpu_sr=0;

	while(1)
	{
		
		Frame_send_type = HeartBeat_Frame;				
		LWIP_Status |= LWIP_SEND_DATA;
		printf("\r\nHeartbeat TASK IS RUNNING\r\n");
		//OS_ENTER_CRITICAL();	//进入临界区(关闭中断)
		//printf("心跳\r\n"); //串口打印结果
		//OS_EXIT_CRITICAL();		//退出临界区(开中断)
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

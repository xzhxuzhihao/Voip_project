/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "udpecho.h"

#include "lwip/opt.h"

#include <lwip/sockets.h>
#include "lwip/api.h"
#include "lwip/sys.h"

#define RECV_DATA         (1024)

static uint8_t str_send[4] = {0xF6,0x28,0x55,0xAA};
/*-----------------------------------------------------------------------------------*/
static void
udpecho_thread(void *arg)
{
  int sock = -1;
  char *recv_data;
  struct sockaddr_in udp_addr,seraddr;
  int recv_data_len;
  socklen_t addrlen;
  
  printf("���ض˿ں���%d\n\n",LOCAL_PORT);
  
  while(1)
  {
    recv_data = (char *)pvPortMalloc(RECV_DATA);
    if (recv_data == NULL)
    {
        printf("No memory\n");
        goto __exit;
    }
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);   //���ú��� Socket ������һ�� Socket��ע�⣺ �ú����ĵڶ������� SOCK_STREAM ��
                                             //ʾ TCP�� SOCK_DGRAM ��ʾ UDP �� SOCK_RAW ��ʾ RAW��
    if (sock < 0)
    {
      printf("Socket error\n");
      goto __exit;
    }
    
    udp_addr.sin_family = AF_INET;                    //IPV4
    udp_addr.sin_addr.s_addr = INADDR_ANY;            //���ñ���IP
    udp_addr.sin_port = htons(LOCAL_PORT);            //���ض˿ں� 
    memset(&(udp_addr.sin_zero), 0, sizeof(udp_addr.sin_zero));
    
    if (bind(sock, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr)) == -1)    //���ú��� bind �����ط�������ַ�� Socket ���а�
    {
      printf("Unable to bind\n");
      goto __exit;
    }
    while(1)
    {
      recv_data_len=recvfrom(sock,recv_data,
                             RECV_DATA,0,
                             (struct sockaddr*)&seraddr,         //���Ͷ�IP
                             &addrlen);
                             
      /*��ʾ���Ͷ˵�IP��ַ*/
      printf("receive from %s\n",inet_ntoa(seraddr.sin_addr));
                             
      /*��ʾ���Ͷ˷������ִ�*/ 
      printf("recevce:%s",recv_data);
      
      if(recv_data_len != 0x00)
	  {
          sendto(sock,&str_send,
                 4,0,
                 (struct sockaddr*)&seraddr,
                 addrlen); 
	  }			 
      /*���ִ����ظ����Ͷ�*/
      sendto(sock,recv_data,
             recv_data_len,0,
             (struct sockaddr*)&seraddr,
             addrlen);
    }
    
__exit:
    if (sock >= 0) closesocket(sock);
    if (recv_data) free(recv_data);
  }
}
/*-----------------------------------------------------------------------------------*/
void
udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, 2048, 4);
}


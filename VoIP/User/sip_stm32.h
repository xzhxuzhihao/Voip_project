/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001-2020 Aymeric MOIZARD amoizard@antisip.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef  _SIP_DP_STM_H
#define  _SIP_DP_STM_H

#define LWIP_DNS 1
#define LWIP_SOCKET 1
#define LWIP_IPV6 1
//#define USE_GETHOSTBYNAME

#include <sockets.h>
#include <netdb.h>


typedef unsigned int socklen_t;


// modified in eXtl_tcp.c from 'val = fcntl(sock, F_GETFL);' to 'val = fcntl(sock, F_GETFL, 0);' 
#define IPPROTO_UDP 17 // In the X86_linux platform is 17
#define IPPROTO_TCP 6 // In the X86_linux platform is 6
#define AF_INET 2

char *strerror_r (int __errnum, char *__buf, size_t __buflen);
//#error No thread implementation found!
//#error No semaphore implementation found!
//#error No thread implementation found!
#endif

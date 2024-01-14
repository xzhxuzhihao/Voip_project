#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "libexosip2-5.3.0/src/eXosip2.h"
#include <osipparser2/headers/osip_to.h>
#include <osip2/osip.h>
#include <osip2/osip_mt.h>
#include <eXosip2/eXosip.h>
#include <eXosip2/eX_message.h>
#include <osipparser2/osip_port.h>

int judge_define(){
#ifdef __arc__
printf("__arc__  def\n");
#else
printf("__arc__  ndef\n");
#endif

#ifdef HAVE_SEMAPHORE_H
printf("HAVE_SEMAPHORE_H  def\n");
#else
printf("HAVE_SEMAPHORE_H  ndef\n");
#endif

#ifdef __APPLE_CC__
printf("__APPLE_CC__  def\n");
#else
printf("__APPLE_CC__  ndef\n");
#endif

#ifdef HAVE_PTHREAD_WIN32
printf("HAVE_PTHREAD_WIN32  def\n");
#else
printf("HAVE_PTHREAD_WIN32  ndef\n");
#endif

#ifdef HAVE_SYS_SEM_H
printf("HAVE_SYS_SEM_H  def\n");
#else
printf("HAVE_SYS_SEM_H  ndef\n");
#endif

#ifdef __VXWORKS_OS__
printf("__VXWORKS_OS__  def\n");
#else
printf("__VXWORKS_OS__  ndef\n");
#endif

#ifdef WINAPI_FAMILY
printf("WINAPI_FAMILY  def\n");
#else
printf("WINAPI_FAMILY  ndef\n");
#endif

#ifdef OSIP_MONOTHREAD
printf("OSIP_MONOTHREAD  def\n");
#else
printf("OSIP_MONOTHREAD  ndef\n");
#endif

#ifdef HAVE_PTHREAD
printf("HAVE_PTHREAD  def\n");
#else
printf("HAVE_PTHREAD  ndef\n");
#endif
}



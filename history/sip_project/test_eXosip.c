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
 
 #include <getopt.h>
 #include <signal.h>
 #include <netdb.h>
 #include <pthread.h>
 #include <sys/types.h>

int eXosip_test_in(struct eXosip_t*excontext){
if(excontext != NULL){
  printf("test_exo:excontext is %p\n", excontext);
  printf("test_exo:excontext->j_reg is %p\n", excontext->j_reg);
}
if(excontext->j_reg != NULL){
  printf("test_exo:excontext->j_reg->r_id is %d\n", excontext->j_reg->r_id);
}
return 0;
}

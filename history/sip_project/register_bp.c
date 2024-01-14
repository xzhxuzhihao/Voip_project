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
int judge_define();
void my_trace_func(const char *fi, int li, osip_trace_level_t level, const char *chfr, va_list ap)
{
    vprintf(chfr, ap);
}
int test_slim(int*slim);

int eXosip_register_send_register_1(eXosip_t *context, int a, osip_message_t *reg);
int	eXosip_test_in(eXosip_t *context);
int eXosip_test(eXosip_t *context);
int eXosip_test_1(eXosip_t *context);
int test_eXosip(struct eXosip_t *context);
int main() {
    eXosip_t *context = eXosip_malloc();
    //eXosip_t *context = (eXosip_t*)malloc(sizeof(eXosip_t));
    //osip_trace_initialize_func(OSIP_INFO4, &my_trace_func);
    //if (eXosip_init(context) != 0) {
    //    printf("Couldn't initialize eXosip!\n");
    //    return -1;
    //}
    //
    //if (eXosip_listen_addr(context, IPPROTO_UDP, NULL, 15060, AF_INET, 0) != 0) {
    //    printf("Couldn't initialize transport layer!\n");
    //    eXosip_quit(context);
    //    return -1;
    //}

	//osip_to_t *to = NULL;
	//osip_to_init(&to);
	//osip_to_parse(to, "sip:wangwei@192.168.0.158");

	//osip_from_t *from = NULL;
	//osip_from_init(&from);
	//osip_from_parse(from, "sip:wangwei@192.168.0.158");

	//char *to_str = NULL;
	//osip_to_to_str(to, &to_str);

	//char *from_str = NULL;
	//osip_from_to_str(from, &from_str);
	//
	//char contact[] = "<sip:wangwei@192.168.0.158:15060;transport=udp>;expires=3600";

    //// Build the REGISTER request
	//printf("t\n");
    //osip_message_t *reg = NULL;
	//eXosip_lock(context);
	//eXosip_reg_t *jr = NULL;
	//printf("ptr: 00context->j_reg = %p\n", context->j_reg);
	//printf("00context->j_reg->r_id = %d\n", context->j_reg->r_id);
	//int result_judge_1 = _eXosip_reg_init(context, &jr, from_str, "sip:192.168.0.158", contact);
	//if(result_judge_1!=0) printf("error.\n");
	////ADD_ELEMENT(context->j_reg, jr);
	//context->j_reg = jr;
	//printf("11context->r_id = %d\n", context->j_reg->r_id);
	//printf("111jr->r_id  = %d\n", jr->r_id);
	//printf("ptr: 111jr  = %p\n", jr);
	//printf("ptr: 112context->j_reg  = %p\n", context->j_reg);
	//jr->r_reg_period = 6000;
	//jr->r_reg_expire = jr->r_reg_period;
	//if (context->auto_masquerade_contact > 0)
    //  jr->registration_step = RS_MASQUERADINGPROCEEDING;

    //int result_judge = eXosip_message_build_request(context, &reg, "REGISTER", NULL, from_str, "sip:192.168.0.158");
	//printf("result_judge in build_request = %d\n", result_judge);

	//osip_message_set_supported(reg, "outbound"); // the Supported field
	//osip_message_set_contact(reg, contact); // the Contact field with an Expires parameter
	//osip_message_set_accept(reg, "application/sdp"); // the Accept field
	//osip_message_set_accept_encoding(reg, "gzip"); // the Accept-Encoding field
	//osip_message_set_accept_language(reg, "en"); // the Accept-Language field
	//osip_message_set_expires(reg, "6000"); // the Expires field    
	//osip_message_set_content_length(reg, "0");
	//									   
    //// Send the REGISTER request
	////if(reg == NULL){
	////int i = _eXosip_register_build_register(context, jr, &reg);
	////  if (i != 0) {
	////	OSIP_TRACE(osip_trace(__FILE__, __LINE__, OSIP_ERROR, NULL, "[eXosip] cannot build REGISTER\n"));
    ////     return i;
	////}
//}
	//if(reg == NULL)
	//	printf("para error\n");
    ////result_judge = eXosip_message_send_request(context, reg);
	////if(result_judge<0) printf("error.\n");
	//if(context->j_reg != NULL)
	//	printf("22 ptr: context = %p\n", context);

    osip_message_t *reg = (osip_message_t *)malloc(sizeof(osip_message_t));
	eXosip_reg_t *jr = NULL;
    //context = (eXosip_t*)malloc(sizeof(eXosip_t));
	jr = (eXosip_reg_t *)malloc(sizeof(eXosip_reg_t));
	context->j_reg = jr;
	int *slim = (int *)malloc(sizeof(slim));
	*slim = 243;
	test_slim(slim);
	int result_judg_2 = eXosip_register_send_register(context, jr->r_id, reg);
	eXosip_register_send_register_1(context, jr->r_id, reg);
	eXosip_test(context);
	eXosip_test_1(context);
	printf("33 ptr: context->j_reg == %p\n", context->j_reg);
	printf("jr->r_id = %d\n", jr->r_id);
	printf("result_judg_2 = %d\n", result_judg_2);
	eXosip_test_in(context);
	if(context == NULL)
		printf("para error\n");
	eXosip_unlock(context);
	
	eXosip_event_t *event;
	while ((event = eXosip_event_wait(context, 2, 500)) != NULL) {
		switch (event->type) {
			case EXOSIP_MESSAGE_NEW:
				printf("New come.\n");
				break;
			case EXOSIP_CALL_INVITE:
				printf("Call invite come.\n");
				break;
			default:
				printf("Others.\n");
				break;
		}
		eXosip_event_free(event);
	}

    //osip_from_free(from);
    //osip_to_free(to);
	//osip_free(to_str);
	//osip_free(from_str);

    eXosip_quit(context);

	printf("\n\n");
	judge_define();

    return 0;
}


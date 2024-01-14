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
int judge_define();
void my_trace_func(const char *fi, int li, osip_trace_level_t level, const char *chfr, va_list ap)
{
    vprintf(chfr, ap);
}

int main() {
    eXosip_t *context = eXosip_malloc();
    osip_trace_initialize_func(OSIP_INFO4, &my_trace_func);
    if (eXosip_init(context) != 0) {
        printf("Couldn't initialize eXosip!\n");
        return -1;
    }
    
    if (eXosip_listen_addr(context, IPPROTO_UDP, NULL, 15060, AF_INET, 0) != 0) {
        printf("Couldn't initialize transport layer!\n");
        eXosip_quit(context);
        return -1;
    }

	osip_to_t *to = NULL;
	osip_to_init(&to);
	osip_to_parse(to, "sip:wangwei@192.168.0.158");

	osip_from_t *from = NULL;
	osip_from_init(&from);
	osip_from_parse(from, "sip:wangwei@192.168.0.158");

	char *to_str = NULL;
	osip_to_to_str(to, &to_str);

	char *from_str = NULL;
	osip_from_to_str(from, &from_str);
	
	char contact[] = "<sip:wangwei@192.168.0.158:15060;transport=udp>;expires=3600";

    // Build the REGISTER request
    osip_message_t *reg = NULL;
    int result_judge = eXosip_message_build_request(context, &reg, "REGISTER", to_str, from_str, "sip:192.168.0.158");
	if(result_judge<0) printf("error.\n");

	osip_message_set_supported(reg, "outbound"); // the Supported field
	osip_message_set_contact(reg, contact); // the Contact field with an Expires parameter
	osip_message_set_accept(reg, "application/sdp"); // the Accept field
	osip_message_set_accept_encoding(reg, "gzip"); // the Accept-Encoding field
	osip_message_set_accept_language(reg, "en"); // the Accept-Language field
	osip_message_set_expires(reg, "3600"); // the Expires field    
										   
    // Send the REGISTER request
    result_judge = eXosip_message_send_request(context, reg);
	if(result_judge<0) printf("error.\n");
	if(context == NULL)
		printf("para error\n");

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

    osip_from_free(from);
    osip_to_free(to);
	osip_free(to_str);
	osip_free(from_str);

    eXosip_quit(context);

	printf("\n\n");
	judge_define();

    return 0;
}


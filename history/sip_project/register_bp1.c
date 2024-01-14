#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <osip2/osip.h>
#include <osip2/osip_mt.h>
#include <eXosip2/eXosip.h>
#include <eXosip2/eX_message.h>
#include <osipparser2/osip_port.h>

void my_trace_func(const char *fi, int li, osip_trace_level_t level, const char *chfr, va_list ap)
{
    vprintf(chfr, ap);
}

int main() {
	int port = 15060;
	int optval;
	int automasquerade = 0;
	int expiry = 3600;
	const char *proxy = "sip:192.168.0.158";
	const char *from = "sip:1002@192.168.0.158";
	const char *contact = "<sip:1002@192.168.0.144:15060;transport=udp>;expires=3600";
	const char *username = "1002";
	const char *password = "1234";
	const char *prog_name = "eXosip2-5.3.0";
	int wait_mtime = 100;
	eXosip_event_t *event;
	
    struct eXosip_t *context = eXosip_malloc();
    osip_trace_initialize_func(OSIP_INFO4, &my_trace_func);
    if (eXosip_init(context) != 0) {
        printf("Couldn't initialize eXosip!\n");
        return -1;
    }
    
    if (eXosip_listen_addr(context, IPPROTO_UDP, NULL, port, AF_INET, 0) != 0) {
        printf("Couldn't initialize transport layer!\n");
        eXosip_quit(context);
        return -1;
    }

	optval = automasquerade;
	eXosip_set_option(context, EXOSIP_OPT_AUTO_MASQUERADE_CONTACT, &optval);
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

    return 0;
}

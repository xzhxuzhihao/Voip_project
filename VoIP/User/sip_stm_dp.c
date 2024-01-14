#include <stdio.h>
#include <sockets.h>

int gettimeofday(struct timeval *tp, void *tz){
	return 0;
}

struct hostent *lwip_gethostbyname(const char *name){
	return NULL;
}
char *strerror_r (int __errnum, char *__buf, size_t __buflen){
	return NULL;
}

#include <time.h>
#include "winguard.h"

int __nsleep(const struct timespec *req, struct timespec *rem)
{
	struct timespec temp_rem;
	if(nanosleep(req,rem)==-1)
		return __nsleep(rem,&temp_rem);
	else
		return 1;
}

int Sleep(unsigned long milisec)
{
	struct timespec req={0,0},rem={0,0};
	time_t sec=(int)(milisec/1000);
	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;
	__nsleep(&req,&rem);
	return 1;
}

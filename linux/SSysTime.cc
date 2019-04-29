#include "SSysTime.h"
#include <sys/time.h>
#include <unistd.h>

SSysTime::SSysTime() {
	TimerFreq = 0;
	TimerBase = 0;
}

SSysTime::~SSysTime() {

}

void SSysTime::Reset(){
	timeval tv;
	gettimeofday(&tv,0);
	TimerBase = (S64) tv.tv_sec*1000000+tv.tv_usec;
}

double SSysTime::GetSeconds(){
	timeval tv;
	gettimeofday(&tv,0);
	S64 usec_total =  (S64) tv.tv_sec*1000000+tv.tv_usec - TimerBase;
	double d = (double) usec_total * (1e-6);
	return d;
}

void SysTimeSleep(unsigned int HowLong) {
	usleep(HowLong * 1000);
}

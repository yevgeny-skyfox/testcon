#include "SSysTime.h"
#include <windows.h>

SSysTime::SSysTime() {
	LARGE_INTEGER F;
	QueryPerformanceFrequency(&F);
	TimerFreq = F.QuadPart;
	TimerBase = 0;
};

SSysTime::~SSysTime() {

};

void SSysTime::Reset() {
	LARGE_INTEGER B;
	if (TimerFreq) {
		QueryPerformanceCounter(&B);
		TimerBase = B.QuadPart;
	} else {
		//high resolution is not supported (old hardware)
		TimerBase = GetTickCount();
	}
}

double SSysTime::GetSeconds() {
	LARGE_INTEGER t;
	double d;
	if (TimerFreq) {
		//high resolution supported
		QueryPerformanceCounter(&t);
		S64 t1 = t.QuadPart - TimerBase;
		d = (double) t1 / (double) TimerFreq;
		return d;
	} else {
		//high resolution is not supported (old hardware)
		S64 d = (GetTickCount() - TimerBase);
		return (double) d * 0.001;
	}
}

void SysTimeSleep(unsigned int HowLong) {
	Sleep(HowLong);
}


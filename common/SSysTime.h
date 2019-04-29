#ifndef __SSYSTIME_H
#define __SSYSTIME_H

#include "types.h"

//! \brief simple timestamp.\n
class SSysTime {
private: 
//! 64 bit timestamp frequency
	S64 TimerFreq;
//! 64 bit base
	S64 TimerBase;
public:
	SSysTime();
//! Reset (set TimerBase to current timestamp)
	void Reset();
//! Get system time from SetBase in ms
	double GetSeconds();
	~SSysTime();
};

void SysTimeSleep(unsigned int HowLong);

#endif

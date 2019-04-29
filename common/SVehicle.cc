#include "SVehicle.h"
#include "SCoord.h"
#include "SSysTime.h"
#include "SThread.h"

#include <stdio.h>
#include <stdlib.h>


THREAD_RETVAL THREAD_CONV VehicleSimulationThreadFunction(void* pArg) {
	SVehicle* pVehicle = (SVehicle*) pArg;
	SSysTime SysTime;
	ERROR_CODE ret = VEHICLE_OK;
	double VirtualTime = 0.0;

	// perform thread own exception handling
	
	try {
		// reset the system time (t = 0 at this point)
		SysTime.Reset();
	
		if (pVehicle->bStartFromBegin) {
			// reset the model to it's default position
			// the second parameter of Step is a flag to initialize
			ret = pVehicle->Step(&VirtualTime, true);
			if (ret != VEHICLE_OK) return (THREAD_RETVAL) ret;
		}
	
		while (pVehicle->isRunning) {
			ret = pVehicle->Step(&VirtualTime, false);
			if (ret != VEHICLE_OK) {
				/* end of file or error */
				pVehicle->isRunning = 0;
				return (THREAD_RETVAL) ret;
				break;
			}
			
			// sync virtual time to system time
			// busy loop for (close to) realtime simulation
			
			while (SysTime.GetSeconds() < VirtualTime) ;
			
		}
		
	} catch (...) {
		pVehicle->isRunning = 0;
		ret = VEHICLE_UNKNOWN_CPP_EXCEPTION;
		return (THREAD_RETVAL) ret;
	}
	
	return (THREAD_RETVAL) ret;
}

void SVehicle::Start(bool bBegin) {
	if (m_pThread == NULL) {
		throw("Start : Vehicle thread not initialized");
	}
	if (!isRunning.exchange(true, std::memory_order_relaxed)) {
		/* start vehicle simulation or reader thread */
		bStartFromBegin = bBegin;
		if (m_pThread->Start(VehicleSimulationThreadFunction, this) != 0) {
			throw("Cannot start vehicle thread");
		}
	}
}

void SVehicle::Resume() {
	SVehicle::Start(false);
}

ERROR_CODE SVehicle::Stop() {
	isRunning = 0;
		
	/* join and return thread exit code (even if already finished) */
	if (m_pThread)
		return m_pThread->WaitForDone();
	
	throw("Stop : Vehicle thread not initialized");
}

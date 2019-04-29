#ifndef _STHREAD_H
#define _STHREAD_H

#include "types.h"

typedef THREAD_RETVAL (THREAD_CONV *THREAD_PROC)(
	void* pThreadParameter
);

//! \brief simple multiplatform threads\n
class SThread {
private:
	//! thread descriptor
	THREAD_HANDLE hThread;
public:
	//! Constructor
	SThread(){hThread = 0;}
	//! Destructor
	~SThread();
	
	//! Thread creation (pThreadProc - pointer to thread function,
	//! pThreadParameter - pointer to function parameter
	//! return: 0=OK, other return values on error
	int Start(THREAD_PROC pThreadProc, void* pThreadParameter);

	//! Wait for thread completion and return Thread's return value (always an error code)
	ERROR_CODE WaitForDone();
};

#endif

#include "SThread.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

SThread::~SThread() {
}

int SThread::Start(THREAD_PROC pThreadProc, void* pThreadParameter){
	if (pthread_create(&hThread, 0, pThreadProc, (void*) pThreadParameter)) {
		hThread = 0;
		return -1;
	}
	return 0;
}

ERROR_CODE SThread::WaitForDone(){
	void* ret_val = 0;
	/* some sleep for schedule */
	usleep(1000);
	pthread_join(hThread, &ret_val);
	hThread = 0;
	return (ERROR_CODE) ret_val;
}



#include "SThread.h"
#include <windows.h>


SThread::~SThread() {
}

int SThread::Start(THREAD_PROC pThreadProc, void* pThreadParameter){
	DWORD ID = 0;
	hThread = CreateThread(NULL,0,pThreadProc,pThreadParameter,0,&ID);
	if (hThread == INVALID_HANDLE_VALUE || hThread == NULL) {
		hThread = 0;
		return -1;
	}
	return 0;
}

DWORD SThread::WaitForDone() {
	Sleep(1);
	DWORD wait_result = WaitForSingleObject(hThread, INFINITE);
	DWORD exit_code = 0;
	if (GetExitCodeThread(hThread, &exit_code)) {
		throw "cannot get thread exit code";
	}
	CloseHandle(hThread);
	hThread=0;
	return exit_code;
}


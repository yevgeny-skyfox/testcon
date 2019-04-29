#include "SGuard.h"
#include <windows.h>

struct GuardPlatformData {
	CRITICAL_SECTION CS;
};

SGuard::SGuard() {
	p_data = new GuardPlatformData;
	if (!p_data) throw "Not enough of memory";
	InitializeCriticalSection(&p_data->CS);
}

SGuard::~SGuard() {
	DeleteCriticalSection(&p_data->CS);
	delete p_data;
}

void SGuard::Lock(){
	EnterCriticalSection(&p_data->CS);
}

void SGuard::Unlock(){
	LeaveCriticalSection(&p_data->CS);
}


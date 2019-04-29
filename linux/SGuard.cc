#include "SGuard.h"
#include <pthread.h>

struct GuardPlatformData {
	pthread_mutex_t mutex;
};

SGuard::SGuard() {
	p_data = new GuardPlatformData;
	if (!p_data) throw "not enough of memory";
	int res = 0;
	res = pthread_mutex_init(&p_data->mutex, 0);
	if (res != 0)  throw "pthread_mutex_init()";
}

SGuard::~SGuard() {
	int res = pthread_mutex_destroy(&p_data->mutex);
	if (res != 0)  throw "pthread_mutex_destroy()";
	delete p_data;
}

void SGuard::Lock(){
	int res = pthread_mutex_lock(&p_data->mutex);
	if (res != 0)  throw "pthread_mutex_lock()";
}

void SGuard::Unlock(){
	int res = pthread_mutex_unlock(&p_data->mutex);
	if (res != 0)  throw "pthread_mutex_unlock()";
}


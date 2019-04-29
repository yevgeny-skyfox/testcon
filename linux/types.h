#ifndef __TYPES_H
#define __TYPES_H

#include <stdint.h>
#include <pthread.h>

typedef int64_t S64;

#define THREAD_CONV
#define THREAD_RETVAL void*
#define THREAD_HANDLE pthread_t

#define ERROR_CODE unsigned long

#endif
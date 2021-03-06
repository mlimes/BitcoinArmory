#include "pthread.h"

int pthread_mutex_init(pthread_mutex_t *mu, const int mutex_attr)
{
	InitializeCriticalSection(mu);

	return 0;
}

DWORD pthread_self()
{
	return GetCurrentThreadId();
}

int pthread_mutex_lock(pthread_mutex_t *mu)
{
	EnterCriticalSection(mu);

	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mu)
{
	LeaveCriticalSection(mu);

	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mu)
{
	DeleteCriticalSection(mu);

	return 0;
}

int pthread_create(pthread_t *tid, pthread_attr_t *attr, void*(*start)(void*), void *arg)
{
	if(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, arg, 0, tid)) return 0;

	return -1;
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	InitializeConditionVariable(cond);

	return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
	WakeConditionVariable(cond);
	
	return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
	WakeAllConditionVariable(cond);
	
	return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mu)
{
	SleepConditionVariableCS(cond, mu, INFINITE);
	return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
	return 0;
}

int pthread_once(pthread_once_t *once, void (*func)(void))
{
	long state = *once;

	_ReadWriteBarrier();
	
	while (state != 1)
	{
		if (!state)
		{
			if (!InterlockedCompareExchange(once, 2, 0))
			{
				func();
				*once = 1;
				
				return 0;
			}
		}
		
		YieldProcessor();
		
		_ReadWriteBarrier();
		
		state = *once;
	}
	
	return 0;
}
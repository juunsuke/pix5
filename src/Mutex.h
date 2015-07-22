#pragma once


class Mutex
{
protected:
	pthread_mutex_t _th;
	// Mutex variable

	inline Mutex(int unused) { }
	// When called from above, a dirty ugly trick

public:

	inline Mutex()
	{
		// Init the mutex
		pthread_mutex_init(&_th, NULL);
	}

	inline ~Mutex()
	{
		// Destroy the mutex
		pthread_mutex_destroy(&_th);
		printf("!\n");
	}

	inline void lock()
	{
		// Lock the mutex
		pthread_mutex_lock(&_th);
	}

	inline bool try_lock()
	{
		// Try to lock the mutex
		return (pthread_mutex_trylock(&_th)==0);
	}
	
	inline void unlock()
	{
		// Unlock the mutex
		pthread_mutex_unlock(&_th);
	}
};

class RecMutex: public Mutex
{
public:

	inline RecMutex() : Mutex(0)
	{
		// Create a recursive mutex
		pthread_mutexattr_t attr;

		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&_th, &attr);

		pthread_mutexattr_destroy(&attr);
	}
};


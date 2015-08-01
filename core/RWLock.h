#pragma once


class RWLock
{
	pthread_rwlock_t _rw;
	// Lock variable

public:

	inline RWLock()
	{
		// Initialize the RW lock
		pthread_rwlock_init(&_rw, NULL);
	}

	inline ~RWLock()
	{
		// Destroy the RW lock
		pthread_rwlock_destroy(&_rw);
	}

	inline void read()
	{
		// Acquire a read lock
		pthread_rwlock_rdlock(&_rw);
	}

	inline bool try_read()
	{
		// Try to acquire a read lock
		return (pthread_rwlock_tryrdlock(&_rw)==0);
	}

	inline void write()
	{
		// Acquire a write lock
		pthread_rwlock_wrlock(&_rw);
	}
	
	inline bool try_write()
	{
		// Try to acquire a write lock
		return (pthread_rwlock_trywrlock(&_rw)==0);
	}

	inline void unlock()
	{
		// Unlock
		pthread_rwlock_unlock(&_rw);
	}
};


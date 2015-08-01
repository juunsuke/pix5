#pragma once


class Condition: public Mutex
{
	pthread_cond_t _cond;
	// Condition variable

public:

	inline Condition()
	{
		// Create the condition
		pthread_cond_init(&_cond, NULL);
	}

	inline ~Condition()
	{
		// Destroy the condition
		pthread_cond_destroy(&_cond);
	}

	inline void signal()
	{
		// Signal the condition
		pthread_cond_signal(&_cond);
	}

	inline void broadcast()
	{
		// Broadcast a signal
		pthread_cond_broadcast(&_cond);
	}

	inline void wait()
	{
		// Block and wait for a signal
		pthread_cond_wait(&_cond, &_th);
	}

	bool wait(int timeout)
	{
		// Block and wait with a specified timeout, in milliseconds
		// Returns true if a signal was recieved, false if the timeout was reached

		// Get current time
		struct timeval now;
		gettimeofday(&now, NULL);

		// Setup absolute timeout
		struct timespec ttw;
		ttw.tv_sec = now.tv_sec;
		ttw.tv_nsec = (now.tv_usec+timeout*1000UL)*1000UL;

		ttw.tv_sec += ttw.tv_nsec/1000000000;
		ttw.tv_nsec %= 1000000000;

		// Do the wait
		return (pthread_cond_timedwait(&_cond, &_th, &ttw)==0);
	}
};


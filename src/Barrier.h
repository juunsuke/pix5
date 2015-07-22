#pragma once


class Barrier
{
	pthread_barrier_t _bar;
	// The barrier variable

public:

	inline Barrier(int count)
	{
		// Initialize the barrier
		pthread_barrier_init(&_bar, NULL, count);
	}

	inline ~Barrier()
	{
		// Destroy the barrier
		pthread_barrier_destroy(&_bar);
	}

	inline void wait()
	{
		// Wait on the barrier
		pthread_barrier_wait(&_bar);
	}
};



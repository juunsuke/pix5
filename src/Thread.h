#pragma once


namespace ThreadState
{
	enum Type
	{
		None,
		// Nothing happened yet

		Running,
		// The thread is running

		Done,
		// The thread has completed successfully

		Error
		// The thread has thrown an error, which was stored in 'thread_error'
	};
}

class Thread: public Obj
{
	pthread_t _th;
	// Thread handle

public:

	volatile ThreadState::Type thread_state;
	// Current thread state
	// Should be considered read-only, it is set internally

	Error *thread_error;
	// If an error was thrown and uncatched, this will be set to the catched error
	// and the state will become ThreadState::Error
	// Otherwise, it is NULL


	inline Thread()
	{
		_th = 0;
		thread_state = ThreadState::None;
		thread_error = NULL;
	}

	virtual inline ~Thread()
	{
		if(thread_error)
			delete thread_error;
	}


	void start_thread();
	// Start the thread
	// This creates a new thread and calls on_thread_start()

	void join();
	// Join the thread
	// If the thread was never started, join will exit silently



	virtual void on_thread_start() = 0;
	// Called when the thread is started
};


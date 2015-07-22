#pragma once


class JobThread: public Thread
{
	class ThreadPool *_pool;
	// The pool that owns this job thread

	JobThread(class ThreadPool *pool);
	~JobThread();
	// Only the pool can instantiate/delete this class

	void on_thread_start();
	// Overloaded start function

	friend class ThreadPool;

public:



};


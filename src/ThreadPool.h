#pragma once

class ThreadPool
{
	List<JobThread*> _threads;
	// The threads in the pool

	FIFO<Job*> _jobs;
	// Jobs FIFO

	Condition _cond;
	// Jobs mutex/condition

	volatile bool _stop, _finish;
	// Wether stop mode is enabled

public:

	ThreadPool(int size);
	~ThreadPool();


	void start();
	// Start all the threads in the pool

	void stop(bool finish);
	// Stop all the threads in the pool
	// If 'finish' is true, the threads will not be stopped until all queued jobs have been processed
	// If 'finish' is false, queued jobs will remain in the queue
	// Either way, all currently running jobs will be allowed to finish, so this call might block for some time

	void add_job(Job *job);
	// Add a new job to the queue

	Job *get_job();
	// Get a job to process
	// The function will block until a job is made available
	// If NULL is returned, the pool is in shutdown mode and
	// the job thread should terminate

};


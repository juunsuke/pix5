#include "pix5.h"

namespace PIX {


ThreadPool::ThreadPool(int size)
{
	ASSERT(size>0, "ThreadPool::ThreadPool(): Invalid size")

	_stop = false;
	_finish = false;

	// Create all the threads
	for(int c = 0; c<size; c++)
		_threads.add(new JobThread(this));
}

ThreadPool::~ThreadPool()
{
	// Join and delete all the threads
	for(int c = 0; c<_threads.size(); c++)
	{
		_threads[c]->join();
		delete _threads[c];
	}

	_threads.clear_nodel();
}
	
void ThreadPool::start()
{
	// Start all the threads
	for(int c = 0; c<_threads.size(); c++)
		_threads[c]->start_thread();
}

void ThreadPool::stop(bool finish)
{
	// Processing must stop
	_finish = finish;
	_stop = true;

	// Wake-up all threads
	_cond.lock();
	_cond.broadcast();
	_cond.unlock();

	// Join all the threads
	for(int c = 0; c<_threads.size(); c++)
		_threads[c]->join();
}

void ThreadPool::add_job(Job *job)
{
	ASSERT(job->job_state!=JobState::Queued, "ThreadPool::add_job(): Job is already queued in a poool")
	ASSERT(job->job_state!=JobState::Running, "ThreadPool::add_job(): Job is already running in a poool")

	_cond.lock();

		// Add a new job to the queue
		job->job_state = JobState::Queued;
		_jobs.add(job);

		// Signal so that a thread can grab it
		_cond.signal();

	_cond.unlock();
}

Job *ThreadPool::get_job()
{
	_cond.lock();

	Job *job = NULL;

	for(;;)
	{
		// Check if a job is available
		if((volatile int)_jobs.count())
		{
			// Grab one
			job = _jobs.get();
			break;
		}

		// No jobs pending
		if(_stop)
			break;

		// Block until a signal is received
		_cond.wait();

		// If stop mode with no finish is enabled, terminate
		if(_stop && !_finish)
			break;
	}
			
	_cond.unlock();
	return job;
}


}

#include "pix5.h"

namespace PIX {

	
JobThread::JobThread(ThreadPool *pool)
{
	_pool = pool;
}

JobThread::~JobThread()
{
}

void JobThread::on_thread_start()
{
	for(;;)
	{
		// Grab a job
		Job *job = _pool->get_job();

		// Stop processing ?
		if(!job)
			break;

		// Do the job
		job->job_state = JobState::Running;

		try
		{
			job->job_start();
			job->job_state = JobState::Done;
		}
		catch(Error e)
		{
			job->job_error = new Error(e);
			job->job_state = JobState::Error;
		}
	}
}


}

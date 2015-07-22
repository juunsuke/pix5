#include "pix5.h"

namespace PIX {


static void *th_start(void *p)
{
	// Start the thread
	Thread *th = (Thread*)p;

	try
	{
		th->on_thread_start();
		th->thread_state = ThreadState::Done;
	}
	catch(Error e)
	{
		// An error occured
		th->thread_error = new Error(e);
		th->thread_state = ThreadState::Error;
	}

	return NULL;
}


void Thread::start_thread()
{
	ASSERT(thread_state!=ThreadState::Running, "Thread::start_thread(): Thread is already running")

	// Start the thread
	thread_state = ThreadState::Running;
	pthread_create(&_th, NULL, th_start, this);
}

void Thread::join()
{
	if(thread_state==ThreadState::Running)
		pthread_join(_th, NULL);

	ASSERT(thread_state==ThreadState::Done || thread_state==ThreadState::Error, "Thread::join(): Unhealthy state detected")
}


}

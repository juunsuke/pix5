#pragma once


namespace JobState
{
	enum Type
	{
		None,
		// No state, job is stand-alone

		Queued,
		// Job is queued in a pool

		Running,
		// Job is currently running

		Done,
		// Job has terminated successfully

		Error
		// Job has terminated with an error
		// job_error will hold the error
	};
}



class Job
{
public:

	volatile JobState::Type job_state;
	// State of the job
	// Should be considered read-only, set internally

	Error *job_error;
	// Error code if an error was thrown during processing


	Job();
	~Job();


	virtual void job_start() = 0;
	// Start the job
};


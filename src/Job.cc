#include "pix5.h"

namespace PIX {


Job::Job()
{
	job_state = JobState::None;
	job_error = NULL;
}

Job::~Job()
{
}




}

#include "pix5input.h"


namespace Input
{
	void init();
	void done();
}

namespace Action
{
	void init();
	void done();
}


static void input_init()
{
	Input::init();
	Action::init();
}

static void input_shutdown()
{
	Action::done();
	Input::done();
}


static void input_reg() __attribute__((constructor(110)));
static void input_reg()
{
	pix_reg_module("PIX Input", input_init, input_shutdown);
}




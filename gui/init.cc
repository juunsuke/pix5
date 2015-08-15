#include "pix5gui.h"


namespace GUI
{
	void init();
	void done();
}



static void gui_init()
{
	GUI::init();
}

static void gui_shutdown()
{
	GUI::done();
}


static void gui_reg() __attribute__((constructor()));
static void gui_reg()
{
	pix_reg_module("PIX GUI", gui_init, gui_shutdown);
}





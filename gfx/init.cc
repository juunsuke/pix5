#include "pix5gfx.h"


namespace Display
{
	void init();
	void done();
}

void font_init();
void font_done();

void vd_init();
void vd_done();




static void gfx_init()
{
	vd_init();
	font_init();
	Display::init();
}

static void gfx_shutdown()
{
	Display::done();
	font_done();
	vd_done();
}


static void gfx_reg() __attribute__((constructor(111)));
static void gfx_reg()
{
	pix_reg_module("PIX Graphics", gfx_init, gfx_shutdown);
}



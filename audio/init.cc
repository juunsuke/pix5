#include "pix5audio.h"


namespace Audio
{
	void init();
	void done();
}



static void audio_init()
{
	Audio::init();
}

static void audio_shutdown()
{
	Audio::done();
}


static void audio_reg() __attribute__((constructor()));
static void audio_reg()
{
	pix_reg_module("PIX Audio", audio_init, audio_shutdown);
}





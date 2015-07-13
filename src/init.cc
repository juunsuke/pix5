#include "pix5.h"
#include <SDL.h>


namespace PIX {


static bool _is_init = false;
// Flag to check wether PIX has been initialized

static Str _app_name;
// Registered app name

static bool _has_parachute = false;
// Wether the parachute has been installed


namespace Display		{ void init(); void done(); }
namespace Input			{ void init(); void done(); }
namespace Audio			{ void init(); void done(); }
namespace Cache			{ void init(); void done(); }
namespace GUI			{ void init(); void done(); }


void font_init();		void font_done();
void sprite_init();		void sprite_done();
void tr_init();			void tr_done();



static void parachute()
{
	// Skip if not initialized
	if(!_is_init)
		return;

	#ifdef DBG
	Log::debug("PIX5 Parachute: Shutdown not called manually... Shutting down");
	#endif

	pix_shutdown();
}

void pix_init(const Str& app_name)
{
	ASSERT(!_is_init, "Double initialization")

	Log::log("Initializing PIX...");

	// Init
	_is_init = true;
	_app_name = app_name;

	// Init SDL
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK|SDL_INIT_EVENTS))
		E::SDL("%s", SDL_GetError());

	// Install the parachute if needed
	if(!_has_parachute)
	{
		atexit(parachute);
		_has_parachute = true;
	}

	Input::init();
	font_init();
	Display::init();
	tr_init();
	sprite_init();
	Audio::init();
	Cache::init();
	GUI::init();

	Log::log("PIX successfully initialized");
}

void pix_shutdown()
{
	// Do nothing if not initialized
	if(!_is_init)
		return;

	Log::log("Shutting down PIX...");

	// Shutdown
	GUI::done();
	Audio::done();
	sprite_done();
	tr_done();
	Display::done();
	font_done();
	Input::done();
	Cache::done();

	SDL_Quit();

	_app_name = "";
	_is_init = false;
}

bool was_init()
{
	return _is_init;
}

Str get_app_name()
{
	return _app_name;
}


}

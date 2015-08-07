#include "pix5core.h"
#include <SDL.h>


static bool _is_init = false;
// Flag to check wether PIX has been initialized

static Str _app_name;
// Registered app name

static bool _has_parachute = false;
// Wether the parachute has been installed


namespace Cache		{ void init(); void done(); }
namespace IO		{ void init(); void done(); }




struct PixModule
{
	char *name;
	void (*init)();
	void (*shutdown)();
};

static PixModule _modules[MAX_PIX_MODULES];
static int _num_modules = 0;


void pix_reg_module(const char *name, void (*init)(), void (*shutdown)())
{
	// Register a new module
	ASSERT(_num_modules<MAX_PIX_MODULES, "pix_reg_module(): Reached the maximum number of modules")

	// Register the module
	PixModule *mod = _modules+_num_modules;
	_num_modules++;

	mod->name = strdup(name);
	mod->init = init;
	mod->shutdown = shutdown;
}


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
	if(SDL_Init(SDL_INIT_EVENTS))
		E::SDL("%s", SDL_GetError());

	// Install the parachute if needed
	if(!_has_parachute)
	{
		atexit(parachute);
		_has_parachute = true;
	}

	IO::init();
	Cache::init();

	// Initialize the modules
	for(int c = 0; c<_num_modules; c++)
	{
		Log::log("Initializing module: %s", _modules[c].name);
		_modules[c].init();
	}

	Log::log("PIX successfully initialized");
}

void pix_shutdown()
{
	// Do nothing if not initialized
	if(!_is_init)
		return;

	Log::log("Shutting down PIX...");

	Cache::done();

	// Shutdown the modules
	for(int c = _num_modules-1; c>=0; c--)
	{
		Log::log("Shutting down module: %s", _modules[c].name);
		_modules[c].shutdown();
	}

	IO::done();

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



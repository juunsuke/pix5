#include "pix5core.h"
#include <SDL.h>


CoreEventHandler::CoreEventHandler()
{
}

CoreEventHandler::~CoreEventHandler()
{
}

void CoreEventHandler::dispatch_event(SDL_Event *ev)
{
	// Dispatch the event
	switch(ev->type)
	{
		case SDL_QUIT:
			// Quit event
			on_quit();
			break;
	}
}
	
void CoreEventHandler::process_events()
{
	// Process and dispatch all queued SDL events
	SDL_Event ev;
	while(SDL_PollEvent(&ev))
		dispatch_event(&ev);
}

void CoreEventHandler::on_quit()
{
	#ifdef DBG
	Log::debug("CoreEventHandler::on_quit() unhandled, forcing shutdown");
	#endif

	pix_shutdown();
	exit(0);
}





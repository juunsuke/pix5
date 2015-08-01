#include "pix5gfx.h"
#include <SDL.h>


namespace Display
{
void handle_resize(int width, int height);
void handle_move(int x, int y);
}



GfxEventHandler::GfxEventHandler()
{
}

GfxEventHandler::~GfxEventHandler()
{
}

void GfxEventHandler::dispatch_event(SDL_Event *ev)
{
	// Dispatch the event
	switch(ev->type)
	{
		case SDL_WINDOWEVENT:
			switch(ev->window.event)
			{
				case SDL_WINDOWEVENT_MOVED:
					// Window was moved
					Display::handle_move(ev->window.data1, ev->window.data2);
					on_window_move(ev->window.data1, ev->window.data2);
					break;
				
				case SDL_WINDOWEVENT_RESIZED:
					// Window was resized
					Display::handle_resize(ev->window.data1, ev->window.data2);
					on_window_resize(ev->window.data1, ev->window.data2);
					break;

				case SDL_WINDOWEVENT_SHOWN:						on_window_show();						break;
				case SDL_WINDOWEVENT_HIDDEN:					on_window_hide();						break;
				case SDL_WINDOWEVENT_MINIMIZED:					on_window_minimize();					break;
				case SDL_WINDOWEVENT_MAXIMIZED:					on_window_maximize();					break;
				case SDL_WINDOWEVENT_RESTORED:					on_window_restore();					break;
				case SDL_WINDOWEVENT_ENTER:						on_window_enter();						break;
				case SDL_WINDOWEVENT_LEAVE:						on_window_leave();						break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:				on_window_focus_gain();					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:				on_window_focus_lose();					break;
			}
			break;

		default:
			// Not a graphics event
			InputEventHandler::dispatch_event(ev);
			break;
	}
}


#include "pix5.h"
#include <SDL.h>


namespace PIX {


namespace Display
{
void handle_resize(int width, int height);
void handle_move(int x, int y);
}

namespace Input
{
void handle_event(EventHandler *eh, SDL_Event *ev);
}


EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}
	
void EventHandler::process_events()
{
	// Process all queued SDL events
	SDL_Event ev;
	while(SDL_PollEvent(&ev))
		switch(ev.type)
		{
			case SDL_QUIT:
				// Quit event
				on_quit();
				break;

			case SDL_WINDOWEVENT:
				switch(ev.window.event)
				{
					case SDL_WINDOWEVENT_MOVED:
						// Window was moved
						Display::handle_move(ev.window.data1, ev.window.data2);
						on_window_move(ev.window.data1, ev.window.data2);
						break;
					
					case SDL_WINDOWEVENT_RESIZED:
						// Window was resized
						Display::handle_resize(ev.window.data1, ev.window.data2);
						on_window_resize(ev.window.data1, ev.window.data2);
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

			case SDL_KEYDOWN:					case SDL_KEYUP:						case SDL_TEXTEDITING:
			case SDL_TEXTINPUT:					case SDL_MOUSEMOTION:				case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:				case SDL_MOUSEWHEEL:				case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:				case SDL_JOYHATMOTION:				case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:				case SDL_JOYDEVICEADDED:			case SDL_JOYDEVICEREMOVED:
			case SDL_CONTROLLERAXISMOTION:		case SDL_CONTROLLERBUTTONDOWN:		case SDL_CONTROLLERBUTTONUP:
			case SDL_CONTROLLERDEVICEADDED:		case SDL_CONTROLLERDEVICEREMOVED:	case SDL_CONTROLLERDEVICEREMAPPED:
			case SDL_FINGERDOWN:				case SDL_FINGERUP:					case SDL_FINGERMOTION:
				// Input events
				// Dispatch them to the input module
				Input::handle_event(this, &ev);
				break;
		}
}

void EventHandler::on_quit()
{
	#ifdef DBG
	Log::debug("EventHandler::on_quit() unhandled, forcing shutdown");
	#endif

	pix_shutdown();
	exit(0);
}




}

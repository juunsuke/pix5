#include "pix5input.h"
#include <SDL.h>


namespace Input
{
void handle_event(InputEventHandler *eh, SDL_Event *ev);
}


InputEventHandler::InputEventHandler()
{
}

InputEventHandler::~InputEventHandler()
{
}

void InputEventHandler::dispatch_event(SDL_Event *ev)
{
	// Dispatch the event
	switch(ev->type)
	{
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
			Input::handle_event(this, ev);
			break;

		default:
			// Not an input event
			CoreEventHandler::dispatch_event(ev);
			break;
	}
}

#include "pix5.h"
#include <SDL.h>


namespace PIX {
namespace Input {


static Mouse _mouse;
// Current mouse state

static Keyboard _kbd;
// Current keyboard state

static bool _repeat = false;
// Auto-repeate for on_key_down events



void reset();


void init()
{
	reset();

	_repeat = false;

	// Setup the key data
	for(int c = 0; c<512; c++)
	{
		// Get basic key info
		SDL_Scancode sc = (SDL_Scancode)c;
		SDL_Keycode key = SDL_GetKeyFromScancode(sc);

		if(!key)
		{
			// No key there
			_kbd.keys[c].code = KEY_NONE;
			_kbd.keys[c].sdl_keycode = 0;
			_kbd.keys[c].code_name = "";
			_kbd.keys[c].key_name = "";
			continue;
		}

		// Setup the key
		_kbd.keys[c].code = (KeyCode)c;
		_kbd.keys[c].sdl_keycode = key;
		_kbd.keys[c].code_name = SDL_GetScancodeName(sc);
		_kbd.keys[c].key_name = SDL_GetKeyName(key);
	}
}

void done()
{
}


void reset()
{
	// Reset the internal state
	// Happens after a mode change or window resize
	_mouse.reset();
	_kbd.reset();
}

Mouse& get_mouse()
{
	return _mouse;
}

Keyboard& get_keyboard()
{
	return _kbd;
}

void move_mouse(int x, int y)
{
	ASSERT(Display::get_window(), "move_mouse(): No mode currently set")

	SDL_WarpMouseInWindow(Display::get_window(), x, y);
}



static void handle_mouse_motion(EventHandler *eh, const SDL_MouseMotionEvent& ev)
{
	// Don't bother if it didn't really move somehow
	if(_mouse.x==ev.x && _mouse.y==ev.y)
		return;

	// Update the position
	_mouse.x = ev.x;
	_mouse.y = ev.y;

	// Send a notification
	eh->on_mouse_move(_mouse);
}

static void handle_mouse_down(EventHandler *eh, const SDL_MouseButtonEvent& ev)
{
	// Ignore out-of-range buttons
	if(ev.button<1 || ev.button>32)
		return;

	// Ignore if the state didn't change somehow
	if(_mouse.button(ev.button))
		return;

	// Mark it as down
	_mouse.buts |= (1<<(ev.button-1));

	// Send a notification
	eh->on_mouse_down(ev.button, _mouse);
}

static void handle_mouse_up(EventHandler *eh, const SDL_MouseButtonEvent& ev)
{
	// Ignore out-of-range buttons
	if(ev.button<1 || ev.button>32)
		return;

	// Ignore if the state didn't change somehow
	if(!_mouse.button(ev.button))
		return;

	// Mark it as down
	_mouse.buts ^= (1<<(ev.button-1));

	// Send a notification
	eh->on_mouse_up(ev.button, _mouse);
}

static void handle_mouse_wheel(EventHandler *eh, const SDL_MouseWheelEvent& ev)
{
	// Mouse wheel action
	if(ev.y==1)
		eh->on_mouse_wheel(false, _mouse);
	else if(ev.y==-1)
		eh->on_mouse_wheel(true, _mouse);
}

static void handle_key_down(EventHandler *eh, const SDL_KeyboardEvent& ev)
{
	// Ignore if the key is already down, unless auto-repeat is on
	Key& key = _kbd.keys[ev.keysym.scancode];
	if(!key.code || (!_repeat && key.down))
		return;

	// Mark it as down
	key.down = true;

	// Send a notification
	eh->on_key_down(key);
}

static void handle_key_up(EventHandler *eh, const SDL_KeyboardEvent& ev)
{
	// Ignore if the key is already up
	Key& key = _kbd.keys[ev.keysym.scancode];
	if(!key.code || !key.down)
		return;

	// Mark it as up
	key.down = false;

	// Send a notification
	eh->on_key_up(key);
}

static void handle_text(EventHandler *eh, const SDL_TextInputEvent& ev)
{
	// Simply report the event
	eh->on_text(ev.text);
}

void handle_event(EventHandler *eh, SDL_Event *ev)
{
	// Dispatch the input event
	switch(ev->type)
	{
		case SDL_MOUSEMOTION:			handle_mouse_motion(eh, ev->motion); break;
		case SDL_MOUSEBUTTONDOWN:		handle_mouse_down(eh, ev->button); break;
		case SDL_MOUSEBUTTONUP:			handle_mouse_up(eh, ev->button); break;
		case SDL_MOUSEWHEEL:			handle_mouse_wheel(eh, ev->wheel); break;
		case SDL_KEYDOWN:				handle_key_down(eh, ev->key); break;
		case SDL_KEYUP:					handle_key_up(eh, ev->key); break;
		case SDL_TEXTINPUT:				handle_text(eh, ev->text); break;
	}
}

void set_key_down_repeat(bool repeat)
{
	_repeat = repeat;
}



}}

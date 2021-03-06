#include "pix5input.h"
#include <SDL.h>



namespace Action
{
	void trigger_key(InputEventHandler *eh, KeyCode key);
	void trigger_mouse(InputEventHandler *eh, int but);
	void trigger_joy_button(InputEventHandler *eh, const Str& joy_name, int but);
	void trigger_joy_hat(InputEventHandler *eh, const Str& joy_name, int hat, HatDirection::Type dir);
}

namespace Input {



static Mouse _mouse;
// Current mouse state

static Keyboard _kbd;
// Current keyboard state

static bool _repeat = false;
// Auto-repeate for on_key_down events

static List<Joystick*> _joys;
// List of connected joysticks

static int _last_joy_id = 0;
// Last used Joystick ID



void reset();

void init()
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	reset();

	_repeat = false;

	_last_joy_id = 0;
}

void done()
{
	// Clear the joysticks
	_joys.clear_del();
}


void reset()
{
	// Reset the internal state
	// Happens after a mode change or window resize
	_mouse.reset();
	_kbd.reset();
	
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

Mouse& get_mouse()
{
	return _mouse;
}

Keyboard& get_keyboard()
{
	return _kbd;
}

static void handle_mouse_motion(InputEventHandler *eh, const SDL_MouseMotionEvent& ev)
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

static void handle_mouse_down(InputEventHandler *eh, const SDL_MouseButtonEvent& ev)
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

	// Check for action triggers
	Action::trigger_mouse(eh, ev.button);
}

static void handle_mouse_up(InputEventHandler *eh, const SDL_MouseButtonEvent& ev)
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

static void handle_mouse_wheel(InputEventHandler *eh, const SDL_MouseWheelEvent& ev)
{
	// Mouse wheel action
	if(ev.y==1)
	{
		eh->on_mouse_wheel(false, _mouse);
		Action::trigger_mouse(eh, 33);
	}
	else if(ev.y==-1)
	{
		eh->on_mouse_wheel(true, _mouse);
		Action::trigger_mouse(eh, 34);
	}
}

static void handle_key_down(InputEventHandler *eh, const SDL_KeyboardEvent& ev)
{
	// Ignore if the key is already down, unless auto-repeat is on
	Key& key = _kbd.keys[ev.keysym.scancode];
	if(!key.code || (!_repeat && key.down))
		return;

	// Mark it as down
	key.down = true;

	// Send a notification
	eh->on_key_down(key);

	// Check for action triggers
	Action::trigger_key(eh, key.code);
}

static void handle_key_up(InputEventHandler *eh, const SDL_KeyboardEvent& ev)
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

static void handle_text(InputEventHandler *eh, const SDL_TextInputEvent& ev)
{
	// Simply report the event
	eh->on_text(ev.text);
}

static void handle_joy_device_added(InputEventHandler *eh, const SDL_JoyDeviceEvent& ev)
{
	// A new joystick was connected
	SDL_Joystick *sj = SDL_JoystickOpen(ev.which);

	// Create a joystick instance
	Joystick *joy = new Joystick();

	// Fill it the info
	joy->id = ++_last_joy_id;
	joy->instance_id = SDL_JoystickInstanceID(sj);
	joy->_sj = sj;
	joy->name = SDL_JoystickName(sj);

	// Populate the axis
	int sz = SDL_JoystickNumAxes(sj);
	for(int c = 0; c<sz; c++)
		joy->axis.add(new JoyAxis(c));

	// Populate the buttons
	sz = SDL_JoystickNumButtons(sj);
	for(int c = 0; c<sz; c++)
		joy->buts.add(new JoyButton(c));

	// Populate the hats
	sz = SDL_JoystickNumHats(sj);
	for(int c = 0; c<sz; c++)
		joy->hats.add(new JoyHat(c));

	// Add the joystick to the list
	_joys.add(joy);

	Log::log("Joystick connected: [%i] %s", joy->id, joy->name.ptr());
	Log::log("   %i axes, %i buttons, %i hats", joy->axis.size(), joy->buts.size(), joy->hats.size());

	// Report
	eh->on_joy_connect(joy->id);
}

static Joystick *get_joy_iid(int iid, int *index = NULL)
{
	// Find a joystick by its instance ID
	for(int c = 0; c<_joys.size(); c++)
	{
		Joystick *joy = _joys[c];

		if(joy->instance_id==iid)
		{
			if(index)
				*index = c;

			return joy;
		}
	}

	// Not found
	return NULL;
}

static Joystick *get_joy_id(int id, int *index = NULL)
{
	// Find a joystick by its ID
	for(int c = 0; c<_joys.size(); c++)
	{
		Joystick *joy = _joys[c];

		if(joy->id==id)
		{
			if(index)
				*index = c;

			return joy;
		}
	}

	// Not found
	return NULL;
}

static void handle_joy_device_removed(InputEventHandler *eh, const SDL_JoyDeviceEvent& ev)
{
	// A joystick was disconnected
	int index;
	Joystick *joy = get_joy_iid(ev.which, &index);
	if(!joy)
	{
		Log::log("Unregistered joystick reported as disconnected: SDL instance ID %i", ev.which);
		return;
	}

	Log::log("Joystick disconnected: [%i] %s", joy->id, joy->name.ptr());

	// Remove it from the list and delete it
	SDL_JoystickClose(joy->_sj);
	_joys.remove_del(index);
	
	eh->on_joy_disconnect(joy->id);
}

static void handle_joy_axis(InputEventHandler *eh, const SDL_JoyAxisEvent& ev)
{
	// Axis motion
	Joystick *joy = get_joy_iid(ev.which);

	if(!joy)
	{
		#ifdef DBG
		Log::debug("handle_joy_axis(): Recieved axis motion event for unknown joystick");
		#endif
		return;
	}

	if((int)ev.axis>=joy->axis.size())
	{
		#ifdef DBG
		Log::debug("handle_joy_axis(): Recieved axis motion event for out-of-bounds axis index");
		#endif
		return;
	}

	JoyAxis *axis = joy->axis[ev.axis];

	// Calc the value, taking dead zone into consideration
	float val = (ev.value<0) ? (float)ev.value/32768.0f : (float)ev.value/32767.0f;

	if(val>0 && val<axis->dead_zone)
		val = 0;
	else if(val<0 && val>-axis->dead_zone)
		val = 0;

	// Do nothing if there is no change
	if(axis->val==val)
		return;

	// Change the value
	axis->val = val;

	// Report
	eh->on_joy_axis(joy->id, axis->index, val);
}

static void handle_joy_button(InputEventHandler *eh, const SDL_JoyButtonEvent& ev)
{
	// Button action
	Joystick *joy = get_joy_iid(ev.which);

	if(!joy)
	{
		#ifdef DBG
		Log::debug("handle_joy_button(): Recieved button event for unknown joystick");
		#endif
		return;
	}

	if((int)ev.button>=joy->buts.size())
	{
		#ifdef DBG
		Log::debug("handle_joy_button(): Recieved button event for out-of-bounds button index");
		#endif
		return;
	}

	JoyButton *but = joy->buts[ev.button];

	// Do nothing if there is no change
	bool val = ev.state==SDL_PRESSED;

	if(but->val==val)
		return;

	// Change the value
	but->val = val;

	// Report
	eh->on_joy_button(joy->id, but->index, val);
	
	// Check for action triggers
	if(joy->mapped_name.len())
		Action::trigger_joy_button(eh, joy->mapped_name, but->index);
}

static void handle_joy_hat(InputEventHandler *eh, const SDL_JoyHatEvent& ev)
{
	// Hat motion
	Joystick *joy = get_joy_iid(ev.which);

	if(!joy)
	{
		#ifdef DBG
		Log::debug("handle_joy_hat(): Recieved hat motion event for unknown joystick");
		#endif
		return;
	}

	if((int)ev.hat>=joy->hats.size())
	{
		#ifdef DBG
		Log::debug("handle_joy_hat(): Recieved hat motion event for out-of-bounds hat index");
		#endif
		return;
	}

	// Get the hat
	JoyHat *hat = joy->hats[ev.hat];

	// Conver the SDL direction to PIX direction
	HatDirection::Type val = HatDirection::Center;

	switch(ev.value)
	{
		case SDL_HAT_LEFTDOWN:			val = HatDirection::DownLeft;		break;
		case SDL_HAT_DOWN:				val = HatDirection::Down;			break;
		case SDL_HAT_RIGHTDOWN:			val = HatDirection::DownRight;		break;

		case SDL_HAT_LEFT:				val = HatDirection::Left;			break;
		case SDL_HAT_RIGHT:				val = HatDirection::Right;			break;
		
		case SDL_HAT_LEFTUP:			val = HatDirection::UpLeft;			break;
		case SDL_HAT_UP:				val = HatDirection::Up;				break;
		case SDL_HAT_RIGHTUP:			val = HatDirection::UpRight;		break;
	}

	// Do nothing if there is no change
	if(hat->val==val)
		return;

	// Change the value
	hat->val = val;

	// Report
	eh->on_joy_hat(joy->id, hat->index, val);
	
	// Check for action triggers
	if(joy->mapped_name.len())
		Action::trigger_joy_hat(eh, joy->mapped_name, hat->index, val);
}

void handle_event(InputEventHandler *eh, SDL_Event *ev)
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

		case SDL_JOYDEVICEADDED:		handle_joy_device_added(eh, ev->jdevice); break;
		case SDL_JOYDEVICEREMOVED:		handle_joy_device_removed(eh, ev->jdevice); break;
		case SDL_JOYAXISMOTION:			handle_joy_axis(eh, ev->jaxis); break;
		case SDL_JOYBUTTONDOWN:			handle_joy_button(eh, ev->jbutton); break;
		case SDL_JOYBUTTONUP:			handle_joy_button(eh, ev->jbutton); break;
		case SDL_JOYHATMOTION:			handle_joy_hat(eh, ev->jhat); break;
	}
}

void set_key_down_repeat(bool repeat)
{
	_repeat = repeat;
}

Joystick get_joystick(int id)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joystick(): Unknown joystick ID: %i", id);

	return *joy;
}

float get_joy_axis(int id, int axis)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joy_axis(): Unknown joystick ID: %i", id);

	if(axis<0 || axis>=joy->axis.size())
		E::BadJoyAxis("Input::get_joy_axis(): Invalid axis index: %i", axis);

	return joy->axis[axis]->val;
}

bool get_joy_button(int id, int but)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joy_button(): Unknown joystick ID: %i", id);

	if(but<0 || but>=joy->buts.size())
		E::BadJoyButton("Input::get_joy_button(): Invalid button index: %i", but);

	return joy->buts[but]->val;
}

HatDirection::Type get_joy_hat(int id, int hat)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joy_hat(): Unknown joystick ID: %i", id);

	if(hat<0 || hat>=joy->hats.size())
		E::BadJoyAxis("Input::get_joy_hat(): Invalid hat index: %i", hat);

	return joy->hats[hat]->val;
}

float get_joy_dead_zone(int id, int axis)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joy_dead_zone(): Unknown joystick ID: %i", id);

	if(axis<0 || axis>=joy->axis.size())
		E::BadJoyAxis("Input::get_joy_dead_zone(): Invalid axis index: %i", axis);

	return joy->axis[axis]->dead_zone;
}

void set_joy_dead_zone(int id, int axis, float dead_zone)
{
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::get_joy_dead_zone(): Unknown joystick ID: %i", id);

	if(axis<0 || axis>=joy->axis.size())
		E::BadJoyAxis("Input::get_joy_dead_zone(): Invalid axis index: %i", axis);

	joy->axis[axis]->dead_zone = Math::fclamp(dead_zone);
}

void map_joystick(const Str& joy_name, int id)
{
	// Give a mapping name to the joystick
	Joystick *joy = get_joy_id(id);
	if(!joy)
		E::BadJoyID("Input::map_joystick(): Unknown joystick ID: %i", id);

	joy->mapped_name = joy_name;

	#ifdef DBG
	Log::debug("Mapped joystick %i to '%s'", id, joy_name.ptr());
	#endif
}

bool is_joy_button_down(const Str& joy_name, int but)
{
	// Check if a joystick with the given name has the given button down
	if(but<0)
		return false;

	for(int c = 0; c<_joys.size(); c++)
	{
		Joystick *joy = _joys[c];

		if(joy->mapped_name==joy_name && but<joy->buts.size() && joy->buts[but]->val)
			return true;
	}

	return false;
}

bool is_joy_hat(const Str& joy_name, int hat, HatDirection::Type dir, bool exclusive)
{
	// Check if a joystick with the given name has the given hat in the given direction
	if(hat<0)
		return false;

	for(int c = 0; c<_joys.size(); c++)
	{
		Joystick *joy = _joys[c];

		if(joy->mapped_name==joy_name && hat<joy->hats.size())
		{
			HatDirection::Type cur = joy->hats[hat]->val;

			if(cur==dir)
				return true;

			if(!exclusive && is_hat_sub(cur, dir))
				return true;
		}
	}

	return false;
}


bool is_hat_sub(HatDirection::Type dir, HatDirection::Type sub)
{
	switch(sub)
	{
		case HatDirection::Left:
			return (dir==HatDirection::Left) || (dir==HatDirection::UpLeft) || (dir==HatDirection::DownLeft);

		case HatDirection::Right:
			return (dir==HatDirection::Right) || (dir==HatDirection::UpRight) || (dir==HatDirection::DownRight);

		case HatDirection::Up:
			return (dir==HatDirection::Up) || (dir==HatDirection::UpLeft) || (dir==HatDirection::UpRight);

		case HatDirection::Down:
			return (dir==HatDirection::Down) || (dir==HatDirection::DownLeft) || (dir==HatDirection::DownRight);

		default:
			return sub==dir;
	}
}

}

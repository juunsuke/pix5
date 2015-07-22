#pragma once

namespace SystemCursor
{
	enum Type
	{
		Arrow,
		IBeam,
		Wait,
		Crosshair,
		WaitArrow,
		SizeNWSE,
		SizeNESW,
		SizeWE,
		SizeNS,
		SizeAll,
		No,
		Hand
	};
}

class Texture;

namespace Input
{



Mouse& get_mouse();
// Get a reference to the internal mouse object

Keyboard& get_keyboard();
// Get a reference to the internal keyboard object

Joystick get_joystick(int id);
// Get a snapshot of the current state of the given joystick ID

float get_joy_axis(int id, int axis);
// Get the current value of a joystick axis

bool get_joy_button(int id, int but);
// Get the current state of a joystick button

HatDirection::Type get_joy_hat(int id, int hat);
// Get the current value of a joystick hat

float get_joy_dead_zone(int id, int axis);
// Get the dead-zone value for the given joystick axis, between 0 and 1

void set_joy_dead_zone(int id, int axis, float dead_zone);
// Set the dead-zone value for the given joystick axis, between 0 and 1



void move_mouse(int x, int y);
// Try to move the mouse cursor to the given location, in window coordinates

void show_cursor(bool show = true);
inline static void hide_cursor() { show_cursor(false); }
// Show/hide the cursor

void set_cursor(SystemCursor::Type cur = SystemCursor::Arrow);
// Set the cursor as a built-in system cursor

void set_cursor(Texture *tex, int hx = 0, int hy = 0, bool mono = false);
// Create a cursor out of a texture and set it
// (hx,hy) is the hotspot location
// The maximum size of the cursor depends on the system
// Not all systems support color cursors
// If 'mono' is true, a mono cursor will be created
// For mono cursors, pixels that are exactly (1,1,1,1) will be
// white, pixels that are exactly (0,0,0,1) will be black, and
// any other pixel will be considered transparant

void set_key_down_repeat(bool repeat);
// Sets wether EventHandler::on_key_down() events will be triggered several
// times when a key remains down for an amount of time (defined by the OS)
// Default is false
// Note that this only affects on_key_down(), on_text() will always repeat



void map_joystick(const Str& joy_name, int id);
// Map the joystick with ID 'id' to the given name

bool is_hat_sub(HatDirection::Type dir, HatDirection::Type sub);
// Check if 'sub' is considered a sub-component of 'dir'
// For instance, returns true if 'sub' is HatDirection::Left and 'dir' is HatDirection::TopLeft


}

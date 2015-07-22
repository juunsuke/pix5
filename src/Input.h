#pragma once

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



void move_mouse(int x, int y);
// Try to move the mouse cursor to the given location, in window coordinates

void set_key_down_repeat(bool repeat);
// Sets wether EventHandler::on_key_down() events will be triggered several
// times when a key remains down for an amount of time (defined by the OS)
// Default is false
// Note that this only affects on_key_down(), on_text() will always repeat



}

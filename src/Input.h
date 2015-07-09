#pragma once

namespace Input
{

Mouse& get_mouse();
// Get a reference to the internal mouse object

Keyboard& get_keyboard();
// Get a reference to the internal keyboard object


void move_mouse(int x, int y);
// Try to move the mouse cursor to the given location, in window coordinates

void set_key_down_repeat(bool repeat);
// Sets wether EventHandler::on_key_down() events will be triggered several
// times when a key remains down for an amount of time (defined by the OS)
// Default is false
// Note that this only affects on_key_down(), on_text() will always repeat



}

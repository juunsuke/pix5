#pragma once


class InputEventHandler: public CoreEventHandler
{
protected:

	void dispatch_event(union SDL_Event *ev);

public:

	InputEventHandler();
	~InputEventHandler();


	virtual void on_mouse_move(Mouse& mouse) { }
	// Called when the mouse moved

	virtual void on_mouse_down(int but, Mouse& mouse) { }
	virtual void on_mouse_up(int but, Mouse& mouse) { }
	// Called when a mouse button is pressed down or released
	// 'but' is an index from 1 to 32, see Mouse for more info

	virtual void on_mouse_wheel_down(Mouse& mouse) { }
	virtual void on_mouse_wheel_up(Mouse& mouse) { }
	virtual void on_mouse_wheel(bool down, Mouse& mouse) { if(down) on_mouse_wheel_down(mouse); else on_mouse_wheel_up(mouse); }
	// Called when the mouse wheel is operated

	virtual void on_key_down(Key& key) { }
	virtual void on_key_up(Key& key) { }
	// Called when a key is depressed/released
	// This is not very suitable for text input, but rather for action buttons

	virtual void on_text(const Str& text) { }
	// Called when a key or another process produces text
	// In most cases, one on_key_down() is followed by one on_text() with
	// a string containing the letter on the key
	// However, this takes into account caps, and international characters
	// When an input method is used (as for Japanese, Chinese, and many more), a large
	// string can be produced through this function, without any on_key_down() events
	// This should be used when text input is required

	virtual void on_joy_connect(int id) { }
	// Called when a new joystick has been connected

	virtual void on_joy_disconnect(int id) { }
	// Called when a joystick has been disconnected

	virtual void on_joy_axis(int id, int axis, float val) { }
	// Called when a joystick axis motion was reported

	virtual void on_joy_button(int id, int but, bool val) { }
	// Called when a joystick button was pressed/released

	virtual void on_joy_hat(int id, int hat, HatDirection::Type val) { }
	// Called when a joystick hat's position has changed

	virtual void on_action(const Str& action) { }
	// Called when an action is triggered

};


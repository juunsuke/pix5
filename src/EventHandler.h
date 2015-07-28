#pragma once


class EventHandler
{
public:

	EventHandler();
	virtual ~EventHandler();


	void process_events();
	// Process queued system events


	//
	// All the PIX global events are defined here
	//

	virtual void on_quit();
	// Called when a quit signal has been received
	// Usually from trying to close the main window
	// Default behaviour: Shut down PIX and terminate the application

	virtual void on_window_resize(int width, int height) { }
	// Called when the window has been resized
	// Usually only happens in Resizable video modes, but can happen otherwise
	// if the operating system or windowing environment decides to force a different size

	virtual void on_window_show() { }
	virtual void on_window_hide() { }
	// Called when the window is shown or hidden

	virtual void on_window_move(int x, int y) { }
	// Called when the window has been moved

	virtual void on_window_minimize() { }
	virtual void on_window_maximize() { }
	virtual void on_window_restore() { }
	// Called when the window has been minimized/maximized/restored

	virtual void on_window_enter() { }
	virtual void on_window_leave() { }
	// Called when the mouse enters/leaves the window

	virtual void on_window_focus_gain() { }
	virtual void on_window_focus_lose() { }
	// Called when the window gains/loses the focus

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



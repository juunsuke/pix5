#pragma once


class GfxEventHandler: public InputEventHandler
{

	void dispatch_event(union SDL_Event *ev);

public:

	GfxEventHandler();
	~GfxEventHandler();


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


};



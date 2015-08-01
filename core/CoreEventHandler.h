#pragma once


class CoreEventHandler
{
protected:

	virtual void dispatch_event(union SDL_Event *ev);
	// Dispatch an event

public:

	CoreEventHandler();
	virtual ~CoreEventHandler();


	void process_events();
	// Process queued system events


	virtual void on_quit();
	// Called when a quit signal has been received
	// Usually from trying to close the main window
	// Default behaviour: Shut down PIX and terminate the application

};



#pragma once


class Game: public EventHandler
{
	PIXOBJ("Game")

	bool _run;
	// Running flag

public:

	Game();
	~Game();


	void run();
	// Run the game, this should only be called once




	//
	// Event handlers
	//

	virtual void on_quit();


	//
	// Game events/callbacks
	//

	virtual Str get_game_name() { return "PixGame"; }
	// Get the game name

	virtual VideoMode get_default_video_mode() { return VideoMode::resizable(1366, 768); }
	// Get the default video mode to be set at startup

	virtual void on_init() {}
	// Called after all basic initializations have been performed and the default video
	// mode has been set
	// The game should initialize itself at this point

	virtual void on_shutdown() {}
	// Called when the game is terminating, before shutting down PIX
	// Guaranteed to be called once and only once if on_init() was called, even if on_init()
	// threw an error

	virtual void on_frame() {}
	// Called once per frame, all rendering should be done here
	// Display::render() will be automatically called after this, so it doesn't have to be
	// manually called.
	// After tha, Display::swap() will be called, and finally events will be processed
};



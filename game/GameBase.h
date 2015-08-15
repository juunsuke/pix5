#pragma once


class GameBase: public GfxEventHandler
{
	Str _game_name;
	// Game name

	bool _run;
	// Main loop run flag

	List<GameScene*> _scene_stack;
	// The scene stack

	GameScene *_scene;
	// The current scene

	GameScene *_set_scene;
	// Scene to set

	GameScene *_call_scene;
	// Scene to call

	bool _end_scene;
	// End scene flag
	
	
	
	void main_loop();
	// Main loop

	void report_error(const Error& e);
	// Report an error

	bool handle_scenes();
	// Handle scene changes


public:

	GameBase(const Str& game_name);
	~GameBase();


	void main(int argc, char **argv);
	void main();
	// Main entry-point

	void quit();
	// Set a flag to terminate the main loop at the next iteration

	

	//
	// Scene management
	//
	// All changes to scenes will not be carried out immediatly when the function is called
	// Rather, the action is queued and will be carried out at the end of the current frame
	// Calling more than one scene function during a single loop iteration will cause an error
	// Scenes are never deleted immediatly, even if the function states it will be deleted
	// The scene will remain valid until the end of the loop, at which point it will be deleted
	//

	void set_scene(GameScene *scene);
	// Set a new current scene
	// The current scene will be deactivated and deleted, but the stack will not be otherwise modified
	// The new scene will be activated and run
	// GameBase takes ownership of the new scene, and will delete it when it terminates

	void call_scene(GameScene *scene);
	// Set a new current scene, preserving the scene stack
	// The current scene will be deactivated, but not deleted
	// The new scene will be activated and run
	// GameBase takes ownership of the new scene, and will delete it when it terminates

	void end_scene();
	// Terminate the current scene
	// The current scene will be deactivated and deleted, and the previous scene in the stack
	// will be re-activated and run


	//
	// Event overrides from GfxEventHandler
	//

	virtual void on_quit() { quit(); }
	// Can be re-overridden
	// The default implementation calls quit(), setting a flag to terminate the main loop


	//
	// Non-event callbacks
	//

	virtual Str game_name() { return _game_name; }
	// Return the game name set in the constructor

	virtual VideoMode get_default_mode() { return VideoMode::resizable(); }
	// Return the default video mode to set during initialization

	virtual GameScene *get_start_scene() { return NULL; }
	// Return the starting scene for the game
	// Although the default implementation returns NULL, this would simply cause the game
	// to immediatly terminate without executing the main loop once



	//
	// Game events
	//

	virtual void on_init() {}
	// Called after PIX5 has been initialized, before starting the main loop
	// When this is called, the startup scene hasn't yet been set, and scene
	// functions shouldn't be called yet

	virtual void on_shutdown() {}
	// Called after exiting the main loop, before shutting down PIX5

	virtual bool on_main_error(const Error& e) { return false; }
	// Called when an unhandled error is catched in the main init/run/done sequence
	// If true is returned, the error is considered handled, and main() will return
	// If false is returned, the error will be reported (on console and in a window), and
	// then main() will return
};


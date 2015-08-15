#include "pix5game.h"
#include <SDL.h>


GameBase::GameBase(const Str& game_name)
{
	_game_name = game_name;
	_scene = NULL;
	_set_scene = NULL;
	_call_scene = NULL;
	_end_scene = false;
}

GameBase::~GameBase()
{
}


void GameBase::report_error(const Error& e)
{
	// On the console
	fprintf(stderr, "%s: Unhandled runtime error: %s\n", _game_name.ptr(), e.format().ptr());

	// Show an error window
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, _game_name, Str::build("Unhandled runtime error:\n%s", e.format().ptr()), NULL);
}

void GameBase::main(int argc, char **argv)
{
	try
	{
		// Initialize PIX5
		pix_init(_game_name);

		// Set the default mode
		Display::set_mode(get_default_mode());

		// Init event
		on_init();

		// Make sure no scene actions were performed in init
		if(_set_scene || _call_scene || _end_scene)
			E::Scene("GameBase::main(): Scene management functions shouldn't be called in on_init()");

		// Get the initial scene
		_scene = get_start_scene();

		// Run the main loop
		if(_scene)
		{
			_scene->on_activate();
			main_loop();
		}
		else
			Log::log("GameBase::get_start_scene() returned NULL, there is nothing to run");

		// Shutdown event
		on_shutdown();
	}
	catch(Error e)
	{
		// Call the user handler
		bool report = !on_main_error(e);

		// Shutdown no matter what
		try
		{
			pix_shutdown();
		}
		catch(Error)
		{
		}

		// Report the error ?
		if(report)
			report_error(e);

		// Done
		return;
	}

	try
	{
		// Shutdown
		pix_shutdown();
	}
	catch(Error e)
	{
		// Shutdown error
		if(!on_main_error(e))
			report_error(e);
	}
}

void GameBase::main()
{
	main(0, NULL);
}

void GameBase::quit()
{
	_run = false;
}

bool GameBase::handle_scenes()
{
	// Handle scene logic
	if(_set_scene)
	{
		// Terminate the current scene
		_scene->on_deactivate();
		delete _scene;

		// Set the new one
		_scene = _set_scene;
		_scene->on_activate();

		_set_scene = NULL;
		return true;
	}

	if(_call_scene)
	{
		// Deactivate and stack the current scene
		_scene->on_deactivate();
		_scene_stack.add(_scene);

		// Set the new scene
		_scene = _call_scene;
		_scene->on_activate();

		_call_scene = NULL;
		return true;
	}

	if(_end_scene)
	{
		// Terminate the current scene
		_scene->on_deactivate();
		delete _scene;

		// Return to the previous scene, if any
		int sz = _scene_stack.size();
		if(sz)
		{
			// Remove it from the stack and activate it
			_scene = _scene_stack[sz-1];
			_scene_stack.remove_nodel(sz-1);

			_scene->on_activate();
		}
		else
		{
			// No more scenes, terminate the loop
			_scene = NULL;
			_run = false;
		}

		_end_scene = false;
		return true;
	}

	return false;
}

void GameBase::main_loop()
{
	// Run the main loop
	_run = true;

	while(_run)
	{
		// Process system events
		process_events();

		// Handle frame logic
		_scene->on_frame();

		// Handle scene changes
		if(handle_scenes())
			continue;

		// Handle rendering
		_scene->on_render();
		Display::render();

		// Swap the display
		Display::swap();
	}

	// Clean up the scenes
	if(_scene)
	{
		_scene->on_deactivate();
		delete _scene;
		_scene = NULL;
	}

	_scene_stack.clear_del();
}

void GameBase::set_scene(GameScene *scene)
{
	if(_set_scene || _call_scene || _end_scene)
		E::Scene("GameBase::set_scene(): Only one scene action can be performed in a single frame");

	if(!scene)
		E::Scene("GameBase::set_scene(): NULL scene provided");

	_set_scene = scene;
}

void GameBase::call_scene(GameScene *scene)
{
	if(_set_scene || _call_scene || _end_scene)
		E::Scene("GameBase::call_scene(): Only one scene action can be performed in a single frame");

	if(!scene)
		E::Scene("GameBase::call_Scene(): NULL scene provided");

	_call_scene = scene;
}

void GameBase::end_scene()
{
	if(_set_scene || _call_scene || _end_scene)
		E::Scene("GameBase::end_scene(): Only one scene action can be performed in a single frame");

	_end_scene = true;
}



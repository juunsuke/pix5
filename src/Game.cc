#include "pix5.h"

namespace PIX {


Game::Game()
{
	_run = false;
}

Game::~Game()
{
}

void Game::run()
{
	bool need_on_shutdown = false;
	
	// Run the game
	try
	{
		// Init PIX
		pix_init(get_game_name());

		// Set the default mode
		Display::set_mode(get_default_video_mode());

		// Perform game-specific initialization
		need_on_shutdown = true;
		on_init();

		// Run the main loop
		for(_run = true; _run; )
		{
			Display::clear(Color(0.2f, 0.3f, 0.9f));

			// Frame event
			on_frame();

			// Render
			Display::render();

			// Swap the display
			Display::swap();

			// Process the events
			process_events();
		}

		// Shutdown
		on_shutdown();
		need_on_shutdown = false;

		pix_shutdown();
	}
	catch(Error e)
	{
		try
		{
			// Emergency shutdown
			if(need_on_shutdown)
				on_shutdown();

			pix_shutdown();
		}
		catch(Error)
		{
		}

		fprintf(stderr, "Unhandled error: %s\n", e.format().ptr());
	}
}

void Game::on_quit()
{
	_run = false;
}


}

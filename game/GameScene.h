#pragma once


class GameScene
{
public:

	GameScene()
	{
	}

	virtual ~GameScene()
	{
	}



	//
	// Scene events
	//

	virtual void on_activate() {}
	// Called when the scene is being activated

	virtual void on_deactivate() {}
	// Called when the scene is being deactivated

	virtual void on_frame() {}
	// Called every logic frame
	// No rendering should occur in this callback, only game logic should be handled
	// It is called after system events have been handled, and before rendering
	
	virtual void on_render() {}
	// Called when a frame should be rendered
	// No logic should occur in this callback, only rendering should be done
	// Display::render() will automatically be called when this function exits, so it is
	// not necessary to manually call it
	// The display will be swapped after this call
};


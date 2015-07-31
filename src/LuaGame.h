#pragma once


class LuaGame: public Game
{
	PIXOBJ("LuaGame")

	Lua _lua;
	// The main lua state

public:

	LuaGame();
	~LuaGame();


	void run(const Str& fname);
	// Run the Lua game
	// 'fname' is the name of the main lua file to load


	//
	// Overloaded functions
	//

	void on_init();
	void on_shutdown();
	void on_frame();

};


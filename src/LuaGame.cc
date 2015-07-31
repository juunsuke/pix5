#include "pix5.h"

namespace PIX {


LuaGame::LuaGame()
{
}

LuaGame::~LuaGame()
{
}


void LuaGame::run(const Str& fname)
{
	// Pre-load the file
	_lua.load_file(fname, false);

	// Pass the control to the base game
	Game::run();
}

void LuaGame::on_init()
{
	// Run the pre-loaded lua file
	if(lua_pcall(_lua, 0, 0, 0))
	{
		Str s = lua_tostring(_lua, -1);
		lua_pop(_lua, 1);
		E::Lua("Runtime error: %s", s.ptr());
	}

	// Call lua's on_init
	if(_lua.get_global_func("on_init") && lua_pcall(_lua, 0, 0, 0))
		_lua.error();
}

void LuaGame::on_shutdown()
{
	// Call lua's on_shutdown
	if(_lua.get_global_func("on_shutdown") && lua_pcall(_lua, 0, 0, 0))
		_lua.error();
}

void LuaGame::on_frame()
{
	// Dispatch to Lua
	if(_lua.get_global_func("on_frame") && lua_pcall(_lua, 0, 0, 0))
		_lua.error();
}


}

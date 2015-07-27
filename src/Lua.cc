#include "pix5.h"
#include <lualib.h>

namespace PIX {


int lua_print(lua_State *lua);



Lua::Lua()
{
	// Create the state
	_lua = lua_open();

	// Load the default libraries
	luaL_openlibs(_lua);

	// Install some core API
	lua_pushlightuserdata(_lua, this);
	lua_pushcclosure(_lua, lua_print, 1);
	//lua_pushcfunction(_lua, lua_print);
	lua_setglobal(_lua, "print");
}

Lua::~Lua()
{
	// Close the state
	lua_close(_lua);
}

void Lua::load(const Str& fname, bool run)
{
	// Load a file
	if(luaL_loadfile(_lua, fname))
		E::Lua("Error loading file: %s", get_error().ptr());

	// Run it ?
	if(run && lua_pcall(_lua, 0, 0, 0))
		E::Lua("%s", get_error().ptr());
}

bool Lua::pcall(int narg, int nres)
{
	return lua_pcall(_lua, narg, nres, 0)==0;
}

LuaData *Lua::get_data(const Str& name)
{
	// Get the global variable
	get_global(name);
	
	// Build a LuaData out of it
	LuaData *ld = new LuaData(this, get_top());
	pop();

	return ld;
}


}

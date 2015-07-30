#include "pix5.h"
#include <lualib.h>

namespace PIX {


extern LuaAPI _core_api[];
// The core API, from LuaAPI.cc


Lua::Lua(bool reg_api)
{
	// Create a new lua state
	_lua = lua_open();
	luaL_openlibs(_lua);

	if(reg_api)
		register_api();
}

Lua::~Lua()
{
	// Close the state
	lua_close(_lua);
}

void Lua::register_api()
{
	// Register the core API
	register_api(_core_api);
}

static void set_mod(lua_State *lua, const char *mod, const char* &last_mod, int& mod_i)
{
	// Global to global
	if(!mod && !last_mod)
		return;

	// No change
	if(mod && last_mod && strcmp(mod, last_mod)==0)
		return;

	// Local to something
	if(last_mod)
		// Remove the table from the stack
		lua_pop(lua, 1);

	// Something to global
	if(!mod)
	{
		last_mod = NULL;
		mod_i = LUA_GLOBALSINDEX;
		return;
	}

	last_mod = mod;
	mod_i = lua_gettop(lua)+1;

	// Check if the table already exists
	lua_getglobal(lua, mod);
	if(lua_istable(lua, -1))
		return;

	if(!lua_isnil(lua, -1))
	{
		lua_pop(lua, 1);
		E::LuaAPI("Lua::register_api(): Global '%s' exists and is not a table", mod);
	}

	lua_pop(lua, 1);

	// Create the table
	lua_newtable(lua);

	// Set the global entry for it
	lua_pushvalue(lua, -1);
	lua_setglobal(lua, mod);
}

void Lua::register_api(LuaAPI *api)
{
	// Check all the entries
	const char *last_mod = NULL;
	int mod_i = LUA_GLOBALSINDEX;

	for(; api->name && api->func; api++)
	{
		// Setup the proper module
		set_mod(_lua, api->mod, last_mod, mod_i);

		// Set the function
		lua_pushcfunction(_lua, api->func);
		lua_setfield(_lua, mod_i, api->name);
	}

	// Unset any set module
	set_mod(_lua, NULL, last_mod, mod_i);

	ASSERT(lua_gettop(_lua)==0, "Lua::register_api(): Stack inconsistancy")
}

void Lua::load_file(const Str& fname, bool run)
{
	// Load the file
	if(luaL_loadfile(_lua, fname))
	{
		Str err = lua_tostring(_lua, -1);
		lua_pop(_lua, 1);

		E::Lua("Error loading Lua file: %s", err.ptr());
	}

	// Run it ?
	if(run)
	{
		if(lua_pcall(_lua, 0, 0, 0))
		{
			Str err = lua_tostring(_lua, -1);
			lua_pop(_lua, 1);

			E::Lua("Runtime error: %s", err.ptr());
		}
	}
}


}

#include "pix5.h"
#include <lualib.h>

namespace PIX {


void luaP_init(lua_State *lua);
void luaP_done(lua_State *lua);


extern LuaAPI _core_api[];
extern LuaConst _core_const[];
// The core API and constants, from LuaAPI.cc


Lua::Lua()
{
	// Create a new lua state
	_lua = lua_open();
	luaL_openlibs(_lua);

	register_api();

	// Init luaP
	luaP_init(_lua);
}

Lua::~Lua()
{
	// Close the state
	luaP_done(_lua);
	lua_close(_lua);
}

void Lua::register_api()
{
	// Register the core API
	register_api(_core_api);
	register_const(_core_const);
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
	
void Lua::register_const(LuaConst *cnst)
{
	// Check all the entries
	const char *last_mod = NULL;
	int mod_i = LUA_GLOBALSINDEX;

	for(; cnst->name; cnst++)
	{
		// Setup the proper module
		set_mod(_lua, cnst->mod, last_mod, mod_i);

		// Set the constant
		lua_pushinteger(_lua, cnst->val);
		lua_setfield(_lua, mod_i, cnst->name);
	}

	// Unset any set module
	set_mod(_lua, NULL, last_mod, mod_i);

	ASSERT(lua_gettop(_lua)==0, "Lua::register_const(): Stack inconsistancy")
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
	if(run && lua_pcall(_lua, 0, 0, 0))
		error();
}

bool Lua::get_global_func(const char *name)
{
	// Find the global function
	lua_getglobal(_lua, name);

	if(!lua_isfunction(_lua, -1))
	{
		// Not found
		lua_pop(_lua, 1);
		return false;
	}

	// Found
	return true;
}

void Lua::error()
{
	// Pop the error message
	Str s = lua_tostring(_lua, -1);
	lua_pop(_lua, 1);

	E::LuaRuntime("%s", s.ptr());
}



}

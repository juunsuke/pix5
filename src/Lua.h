#pragma once


class Lua: public Obj
{
	PIXOBJ("Lua")

	lua_State *_lua;


	void register_api();

public:



	Lua(bool reg_api = true);
	~Lua();
	

	inline operator lua_State*() { return _lua; }
	// Get the state


	void register_api(LuaAPI *api);
	// Register an API definition
	// 'api'  is an array of LuaAPI entries, the last one containing NULL entries

	void load_file(const Str& fname, bool run = true);
	// Load the given Lua source file and place the function on top of the stack
	// If 'run' is true, the function is immediatly called

	bool get_global_func(const char *name);
	// Get the global function with the given name and leave it on the stack
	// Returns 'true' if the function was retrieved, or 'false' otherwise
	// If not retrieved, the stack remains unchanged

	void error();
	// Get an error string from the top of the stack, pop it, and throw it as E::Lua
};


#pragma once


class Lua
{
	lua_State *_lua;

	void register_api();

public:

	Lua(bool reg_api = true);
	~Lua();

	void register_api(LuaAPI *api);
	// Register an API definition
	// 'api'  is an array of LuaAPI entries, the last one containing NULL entries

	void load_file(const Str& fname, bool run = true);
	// Load the given Lua source file and place the function on top of the stack
	// If 'run' is true, the function is immediatly called
};


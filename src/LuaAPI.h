#pragma once


#define LUAF(x) int x(lua_State *lua)

struct LuaAPI
{
	const char *mod;
	// Module name

	const char *name;
	// Function name

	lua_CFunction func;
	// Actual function
};



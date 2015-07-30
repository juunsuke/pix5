#pragma once


struct LuaAPI
{
	const char *mod;
	// Module name

	const char *name;
	// Function name

	lua_CFunction func;
	// Actual function
};



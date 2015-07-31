#include "pix5.h"

namespace PIX {


LUAF(la_print)
{
	// Get the parameter count
	int num = lua_gettop(lua);

	// Convert each parameter to a string and append them together
	Str res;

	for(int c = 1; c<=num; c++)
	{
		// Get the tostring function
		lua_getglobal(lua, "tostring");
		lua_pushvalue(lua, c);

		// Call it
		if(lua_pcall(lua, 1, 1, 0))
			lua_error(lua);

		// Add spacing if needed
		if(c>1)
			res += Str("        ").left(8-res.len()%8);

		res += lua_tostring(lua, -1);
		lua_pop(lua, 1);
	}

	// Output the result
	Log::plog("LUA", res);
	return 0;
}



}

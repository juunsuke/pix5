#include "pix5.h"

namespace PIX {


int lua_print(lua_State *ls)
{
	Lua *lua = Lua::get(ls);

	int num = lua->get_top();

	// Convert each parameter to a string and append them together
	Str res;

	for(int c = 1; c<=num; c++)
	{
		lua->get_global("tostring");
		lua->push_value(c);

		if(!lua->pcall(1, 1))
		{
			// Some error
			lua->remove(-2);
			lua->error();
		}

		// Add spacing if needed
		if(c>1)
			res += Str("        ").left(8-res.len()%8);

		res += lua->to_string();
		lua->pop();
	}

	Log::plog("LUA", res);

	return 0;
	/*int num = lua_gettop(lua);

	// Convert each parameter to a string and append them together
	Str res;

	for(int c = 1; c<=num; c++)
	{
		lua_getglobal(lua, "tostring");
		lua_pushvalue(lua, c);

		if(lua_pcall(lua, 1, 1, 0))
		{
			// Some error
			lua_remove(lua, -2);
			lua_error(lua);
		}

		// Add spacing if needed
		if(c>1)
			res += Str("        ").left(8-res.len()%8);

		res += lua_tostring(lua, -1);
		lua_pop(lua, 1);
	}

	Log::plog("LUA", res);

	return 0;*/
}



}


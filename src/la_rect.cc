#include "pix5.h"

namespace PIX {



LUAF(la_rect_new)
{
	// Grab the 4 components of the rectangle
	int x = luaL_checkint(lua, 1);
	int y = luaL_checkint(lua, 2);
	int w = luaL_checkint(lua, 3);
	int h = luaL_checkint(lua, 4);

	// Build a table out of it
	lua_newtable(lua);

	lua_pushinteger(lua, x);
	lua_setfield(lua, -2, "x");

	lua_pushinteger(lua, y);
	lua_setfield(lua, -2, "y");

	lua_pushinteger(lua, w);
	lua_setfield(lua, -2, "w");

	lua_pushinteger(lua, h);
	lua_setfield(lua, -2, "h");

	return 1;
}




}


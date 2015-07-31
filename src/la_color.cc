#include "pix5.h"

namespace PIX {


LUAF(la_Color)
{
	// Get the color components
	float r = luaL_checknumber(lua, 1);
	float g = luaL_checknumber(lua, 2);
	float b = luaL_checknumber(lua, 3);
	float a = lua_isnumber(lua, 4) ? luaL_checknumber(lua, 4) : 1.0f;

	// Build a color and push it
	lua_pushinteger(lua, (uint32_t)Color(r, g, b, a));

	return 1;
}



}

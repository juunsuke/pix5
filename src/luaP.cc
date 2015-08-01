#include "pix5.h"

namespace PIX {



extern luaL_Reg _texture_api[];


static void reg_meta_api(lua_State *lua, const char *name, const char *index, luaL_Reg *api)
{
	// Create a new metatable
	luaL_newmetatable(lua, "PIX::Texture");

	// Add the special API functions
	for(; api->name && api->func; api++)
	{
		// Push the function
		lua_pushcfunction(lua, api->func);

		// Add it to the metatable
		lua_setfield(lua, -2, api->name);
	}

	// Get the index table, if any
	if(index)
	{
		lua_getglobal(lua, index);

		// Set the __index field of the metatable
		lua_setfield(lua, -2, "__index");
	}

	// Pop the metatable
	lua_pop(lua, 1);
}


void luaP_init(lua_State *lua)
{
	// Register the metatables and their APIs
	reg_meta_api(lua, "PIX::Texture", "Texture", _texture_api);

	ASSERT(lua_gettop(lua)==0, "Stack inconsistency")
}

void luaP_done(lua_State *lua)
{
}

void *luaP_createudata(lua_State *lua, int size, const char *name)
{
	// Create a new user data
	void *p = lua_newuserdata(lua, size);

	// Associate the metatable
	luaL_getmetatable(lua, name);
	if(lua_isnil(lua, -1))
	{
		lua_pop(lua, 1);
		E::Assert("luaP_createudata(): Unknown registry metatable name: '%s'", name);
	}

	// Set it
	lua_setmetatable(lua, -2);

	// Return the pointer
	return p;
}

void luaP_createudata(lua_State *lua, const char *name, void *p)
{
	// Create a userdata with the size of a pointer
	void **ptr = (void**)luaP_createudata(lua, sizeof(void*), name);

	// Set the instance pointer
	*ptr = p;
}

static void *get_udata(lua_State *lua, int narg, const char *name)
{
	// Get/check the userdata
	void *p = luaL_checkudata(lua, narg, name);

	// Make sure it isn't NULL
	luaL_argcheck(lua, p!=NULL, narg, "Invalid object (is NULL)");

	return p;
}

Texture *luaP_checktexture(lua_State *lua, int narg)
{
	return *(Texture**)get_udata(lua, narg, "PIX::Texture");
}

Rect luaP_checkrect(lua_State *lua, int narg)
{
	luaL_argcheck(lua, lua_istable(lua, narg), narg, "Rect expected");

	lua_getfield(lua, narg, "x");
	int x = luaL_checkint(lua, -1);

	lua_getfield(lua, narg, "y");
	int y = luaL_checkint(lua, -1);

	lua_getfield(lua, narg, "w");
	int w = luaL_checkint(lua, -1);

	lua_getfield(lua, narg, "h");
	int h = luaL_checkint(lua, -1);

	return Rect(x, y, w, h);
}




}
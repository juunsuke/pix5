#include "pix5.h"

namespace PIX {


static LUAF(la_texture_tostring)
{
	Texture *tex = luaP_checktexture(lua, 1);

	// Stringize a texture
	lua_pushfstring(lua, "{PIX::Texture} %s (%dx%d)", tex->file().ptr(), tex->width(), tex->height());
	
	return 1;
}

LUAF(la_texture_load)
{
	const char *fname = luaL_checkstring(lua, 1);

	// Load a texture
	Texture *tex = NULL;

	try
	{
		tex = Texture::load(fname);
	}
	catch(Error e)
	{
		luaL_error(lua, "%s", e.format().ptr());
	}

	// Create a new texture object
	luaP_createudata(lua, "PIX::Texture", tex);

	return 1;
}

LUAF(la_texture_create)
{
	int w = luaL_checkint(lua, 1);
	int h = luaL_checkint(lua, 2);
	bool clear = lua_isboolean(lua, 3) ? lua_toboolean(lua, 3) : true;

	// Create a texture
	Texture *tex = NULL;

	try
	{
		tex = Texture::create(w, h, clear);
	}
	catch(Error e)
	{
		luaL_error(lua, "%s", e.format().ptr());
	}

	// Create a new texture object
	luaP_createudata(lua, "PIX::Texture", tex);

	return 1;
}


LUAF(la_texture_delete)
{
	Texture *tex = luaP_checktexture(lua, 1);

	// Delete a texture
	delete tex;

	return 0;
}

LUAF(la_texture_sub)
{
	Texture *tex = luaP_checktexture(lua, 1);
	Rect r = luaP_checkrect(lua, 2);

	// Create a sub-texture
	Texture *subtex = NULL;

	try
	{
		subtex = tex->sub(r);
	}
	catch(Error e)
	{
		luaL_error(lua, "%s", e.format().ptr());
	}

	// Create a new texture object
	luaP_createudata(lua, "PIX::Texture", subtex);

	return 1;
}

LUAF(la_texture_file)
{
	Texture *tex = luaP_checktexture(lua, 1);

	lua_pushstring(lua, tex->file());
	return 1;
}

LUAF(la_texture_width)
{
	Texture *tex = luaP_checktexture(lua, 1);

	lua_pushinteger(lua, tex->width());
	return 1;
}

LUAF(la_texture_height)
{
	Texture *tex = luaP_checktexture(lua, 1);

	lua_pushinteger(lua, tex->height());
	return 1;
}

LUAF(la_texture_is_dirty)
{
	Texture *tex = luaP_checktexture(lua, 1);

	lua_pushboolean(lua, tex->is_dirty());
	return 1;
}


LUAF(la_texture_clear)
{
	Texture *tex = luaP_checktexture(lua, 1);
	Color col = lua_isnumber(lua, 2) ? Color((uint32_t)lua_tonumber(lua, 2)) : Color(0);

	// Clear the texture
	tex->clear(col);

	return 0;
}


LUAF(la_texture_draw)
{
	Texture *tex = luaP_checktexture(lua, 1);

	int dx = luaL_checkint(lua, 2);
	int dy = luaL_checkint(lua, 3);

	try
	{
		tex->draw(dx, dy);
	}
	catch(Error e)
	{
		luaL_error(lua, "%s", e.format().ptr());
	}

	return 0;
}





luaL_Reg _texture_api[] =
{
	{ "__tostring",			la_texture_tostring		},

	{ NULL, NULL },
};


}

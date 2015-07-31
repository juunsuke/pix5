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


static LUAF(la_texture_draw)
{
	Texture *tex = luaP_checktexture(lua, 1);

	int dx = luaL_checkint(lua, 2);
	int dy = luaL_checkint(lua, 3);

	tex->draw(dx, dy);

	return 0;
}




luaL_Reg _texture_api[] =
{
	{ "__tostring",			la_texture_tostring		},
	{ "draw",				la_texture_draw			},

	{ NULL, NULL },
};


}

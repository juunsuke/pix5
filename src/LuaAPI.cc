#include "pix5.h"

namespace PIX {


LUAF(la_print);

LUAF(la_Color);

LUAF(la_rect_new);

LUAF(la_texture_load);
LUAF(la_texture_create);
LUAF(la_texture_delete);
LUAF(la_texture_sub);
LUAF(la_texture_file);
LUAF(la_texture_width);
LUAF(la_texture_height);
LUAF(la_texture_is_dirty);
LUAF(la_texture_clear);
LUAF(la_texture_draw);



//
// Core API
//

LuaAPI _core_api[] =
{
	// la_print.cc
	{ NULL,					"print",					la_print						},

	// la_color.cc
	{ NULL,					"Color",					la_Color						},

	// la_rect.cc
	{ "Rect",				"new",						la_rect_new						},

	// la_texture.cc
	{ "Texture",			"load",						la_texture_load					},
	{ "Texture",			"create",					la_texture_create				},
	{ "Texture",			"delete",					la_texture_delete				},
	{ "Texture",			"sub",						la_texture_sub					},
	{ "Texture",			"file",						la_texture_file					},
	{ "Texture",			"width",					la_texture_width				},
	{ "Texture",			"height",					la_texture_height				},
	{ "Texture",			"is_dirty",					la_texture_is_dirty				},
	{ "Texture",			"clear",					la_texture_clear				},
	{ "Texture",			"draw",						la_texture_draw					},


	{ NULL, NULL, NULL },
};


//
// Constants
//

LuaConst _core_const[] =
{
	{ "TextureFilter",		"Nearest",					0			},
	{ "TextureFilter",		"Linear",					1			},
};




}

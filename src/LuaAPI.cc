#include "pix5.h"

namespace PIX {


LUAF(la_print);
LUAF(la_texture_load);


//
// Core API
//

LuaAPI _core_api[] =
{
	{ NULL,					"print",					la_print						},
	{ "texture",			"load",						la_texture_load					},


	{ NULL, NULL, NULL },
};




}

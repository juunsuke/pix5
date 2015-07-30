#include "pix5.h"

namespace PIX {


int la_print(lua_State *lua);


//
// Core API
//

LuaAPI _core_api[] =
{
	{ NULL,					"print",					la_print						},


	{ NULL, NULL, NULL },
};



}

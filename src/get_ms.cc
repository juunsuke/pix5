#include "pix5.h"
#include <SDL.h>

namespace PIX {


int64_t get_ms()
{
	return SDL_GetTicks();
}


}

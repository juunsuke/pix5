#include "pix5.h"

namespace PIX {



Glyph::Glyph()
{
	width = 0;
	height = 0;
	data = NULL;
}

Glyph::~Glyph()
{
	if(data)
		free(data);
}



}



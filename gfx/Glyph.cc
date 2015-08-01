#include "pix5gfx.h"


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





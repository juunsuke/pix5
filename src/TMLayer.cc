#include "pix5.h"

namespace PIX {



TMLayer::TMLayer(LayerType::Type type, int w, int h)
{
	// Basic setup
	_index = 0;
	_type = type;
	_w = w;
	_h = h;

	// Clear the union
	_tiles = NULL;
	_ts = NULL;
	_va = NULL;
	_dirty = NULL;
}

TMLayer::~TMLayer()
{
	// Free the appropriate data
	switch(_type)
	{
		case LayerType::Tiles:
			// Free the tiles array
			if(_tiles)
				free(_tiles);

			// Free the vertex arrays and dirty flags
			if(_va)
			{
				for(int c = 0; c<_h; c++)
					if(_va[c])
						delete _va[c];

				free(_va);
			}

			if(_dirty)
				free(_dirty);

			break;

		case LayerType::Sprites:
			// Sprites layer
			
			break;

		case LayerType::Int32:
		case LayerType::Int64:
			// Free either integer array
			// Since they're in a union, either _i32 or _i64 are the same
			if(_i32)
				free(_i32);

			break;

		case LayerType::Str:
			// String array
			if(_str)
			{
				//free each one individually
				for(int c = 0; c<((_w*_h)); c++)
					if(_str[c])
						delete _str[c];

				// Free the global array
				free(_str);
			}
			break;
	}
}



}

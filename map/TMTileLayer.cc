#include "pix5map.h"



TMTileLayer::TMTileLayer(int mw, int mh, int tw, int th, Texture *tex) : TMLayer(mw, mh, tw, th)
{
	// Make sure the texture is of proper size
	ASSERT((tex->width()%tw)==0, "TMTileLayer::TMTileLayer(): Tileset width is not divisible by the tile width")
	ASSERT((tex->height()%th)==0, "TMTileLayer::TMTileLayer(): Tileset height is not divisible by the tile height")

	// Setup the texture and calculate metrics
	_tex = tex;
	_tpr = _tex->width()/tw;

	// Allocate the tiles buffer
	_data.resize(mw*mh, true);

	#ifdef DBG
	Log::debug("Added tile layer, tileset contains %i tiles of size %ix%i", _tpr*tex->height()/th, tw, th);
	#endif

	for(int c = 0; c<_data.size(); c++)
		_data[c] = rng.rnd(0, 170);
}

TMTileLayer::~TMTileLayer()
{
}

void TMTileLayer::draw(int dx, int dy, int tx1, int ty1, int tw, int th)
{
	// Draw the tiles
	uint16_t *ptr = _data.ptr() + ty1*width() + tx1;

	int ox = dx;

	int twp = tile_width();
	int thp = tile_height();


	for(int y = 0; y<th; y++)
	{
		for(int x = 0; x<tw; x++)
		{
			// Draw the tile
			int t = (int)ptr[x];
			int tx = t%_tpr;
			int ty = t/_tpr;

			_tex->draw(dx, dy, Rect(tx*twp, ty*thp, twp, thp));

			dx += twp;
		}

		// Skip to the next row
		ptr += width();
		dy += tile_height();
		dx = ox;
	}
}



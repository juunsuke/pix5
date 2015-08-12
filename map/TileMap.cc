#include "pix5map.h"


TileMap::TileMap()
{
	reset();
}

TileMap::TileMap(int mw, int mh, int tw, int th)
{
	reset();
	reset(mw, mh, tw, th);
}

TileMap::~TileMap()
{
	release();
}

void TileMap::reset()
{
	_mw = 0;
	_mh = 0;
	_tw = 0;
	_th = 0;
}

void TileMap::release()
{
	_layers.clear_del();
}

void TileMap::reset(int mw, int mh, int tw, int th)
{
	// Release the old data
	release();

	ASSERT(mw>0 && mh>0, "TileMap::reset(): Invalid map size")
	ASSERT(tw>0 && th>0, "TileMap::reset(): Invalid tile size")

	// Copy the parameters
	_mw = mw;
	_mh = mh;
	_tw = tw;
	_th = th;
}

int TileMap::add_layer(TMLayer *layer)
{
	// Make sure the map has been initialized
	if(_mw==0)
		E::TileMap("TileMap::add_layer(): The map has not been initialized");

	// Make sure the layer parameters match
	if(layer->_mw!=_mw || layer->_mh!=_mh)
		E::TileMap("TileMap::add_layer(): Map size mismatch between map and layer");

	if(layer->_tw!=_tw || layer->_th!=_th)
		E::TileMap("TileMap::add_layer(): Tile size mismatch between map and layer");

	// Add the layer and return its index
	return _layers.add(layer);
}

int TileMap::add_tile_layer(Texture *tex)
{
	// Create a tile layer and add it
	TMTileLayer *lay = new TMTileLayer(_mw, _mh, _tw, _th, tex);

	try
	{
		return add_layer(lay);
	}
	catch(Error)
	{
		delete lay;
		throw;
	}
}

void TileMap::draw(int mx, int my, const Rect& dest)
{
	// Calc the range of visible tiles
	if(dest.w<=0 || dest.h<=0)
		return;

	// First tile to draw on the top-left
	int tx1 = mx/_tw;
	int ty1 = my/_th;

	// Position of the first tile on screen
	int dx = dest.x-(mx%_tw);
	int dy = dest.y-(my%_th);

	// Number of tiles to draw on either direction
	int tw = (dest.w+(mx%_tw)+_tw-1)/_tw;
	int th = (dest.h+(my%_th)+_th-1)/_th;

	// Clip
	if(tx1<0)
	{
		tw += tx1;
		dx += (-tx1)*_tw;
		tx1 = 0;
	}
	if(ty1<0)
	{
		th += ty1;
		dy += (-ty1)*_th;
		ty1 = 0;
	}

	if((tx1+tw)>_mw)
		tw = _mw-tx1;
	if((ty1+th)>_mh)
		th = _mh-ty1;

	if(tw<=0 || th<=0)
		return;

	// Draw every layer
	for(int c = 0; c<_layers.size(); c++)
	{
		TMLayer *lay = _layers[c];

		if(lay->_visible)
			lay->draw(dx, dy, tx1, ty1, tw, th);
	}
}

void TileMap::draw(int mx, int my)
{
	// Draw fullscreen
	VideoMode vm = Display::get_mode();
	draw(mx, my, Rect(0, 0, vm.width, vm.height));
}

uint16_t TileMap::get_tile(int layer, int x, int y)
{
	// Clip
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		return 0;

	if(layer<0 || layer>=_layers.size())
		return 0;

	TMTileLayer *lay = dynamic_cast<TMTileLayer*>(_layers[layer]);
	if(!lay)
		return 0;

	return lay->_data[y*_mw+x];
}

void TileMap::set_tile(int layer, int x, int y, uint16_t t)
{
	// Clip
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		return;

	if(layer<0 || layer>=_layers.size())
		return;

	TMTileLayer *lay = dynamic_cast<TMTileLayer*>(_layers[layer]);
	if(!lay)
		return;

	lay->_data[y*_mw+x] = t;
}






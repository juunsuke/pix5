#include "pix5.h"
#include "data.h"

namespace PIX {


struct BaseTile
{
	int x, y;
	uint32_t tile;
	uint32_t col[4];
};

struct PosVertex
{
	float x, y;
};

struct TileVertex
{
	uint32_t col;
	float u, v;
};


TMBase::TMBase(int mw, int mh, int tw, int th, int tsz)
{
	ASSERT(mw>=1 && mh>=1, "TMBase::TMBase(): Invalid map dimensions")
	ASSERT(tw>=1 && th>=1, "TMBase::TMBase(): Invalid tile dimensions")

	_mw = mw;
	_mh = mh;
	_tw = tw;
	_th = th;
	_tsz = tsz;

	// Load the shader
	_shad = Shader::create_from_source("TileMapShader", (char*)data_sprite_vs, data_sprite_vs_len, (char*)data_sprite_fs, data_sprite_fs_len);
}

TMBase::~TMBase()
{
	_layers.clear_del();

	if(_shad)
		delete _shad;
}

TMLayer *TMBase::add_layer(LayerType::Type type)
{
	// Create a new layer
	TMLayer *lay = new TMLayer(type, _mw, _mh);

	// Set its index and add it to the list
	lay->_index = _layers.add(lay);

	return lay;
}

int TMBase::add_layer_tiles(Texture *ts)
{
	// Validate the texture
	ASSERT((ts->width()%_tw)==0, "TMBase::add_layer_tiles(): The provided texture's width is not divisible by the tile width")
	ASSERT((ts->height()%_th)==0, "TMBase::add_layer_tiles(): The provided texture's height is not divisible by the tile height")

	// Add a tiles layer
	TMLayer *lay = add_layer(LayerType::Tiles);

	// Setup the tiles array
	lay->_tiles = calloc(_mw*_mh, _tsz);

	// Setup the tiles coordinates
	uint8_t *ptr = (uint8_t*)lay->_tiles;
	for(int y = 0; y<_mh; y++)
		for(int x = 0; x<_mw; x++)
		{
			BaseTile *bt = (BaseTile*)ptr;
			bt->x = x;
			bt->y = y;
			bt->col[0] = 0xFFFFFFFF;
			bt->col[1] = 0xFFFFFFFF;
			bt->col[2] = 0xFFFFFFFF;
			bt->col[3] = 0xFFFFFFFF;

			ptr += _tsz;
		}

	// Setup the tileset
	lay->_ts = ts;
	lay->_tx = ts->width()/_tw;
	lay->_ty = ts->height()/_th;
	lay->_uf = 1.0f / (float)lay->_tx;
	lay->_vf = 1.0f / (float)lay->_ty;

	// Prepare the vertex definitions
	VertexDef vd1;
	vd1.add(VertexComp::float2("pos", 0));

	VertexDef vd2;
	vd2.add(VertexComp::ubyte4("col", true, 0));
	vd2.add(VertexComp::float2("texcoord", 4));

	// Create the vertex arrays
	lay->_va = (VertexArray**)malloc(_mh*sizeof(VertexArray*));
	for(int y = 0; y<_mh; y++)
	{
		// Create the VA
		VertexArray *va = new VertexArray();

		// Add the 2 streams
		va->add_stream(vd1, sizeof(PosVertex), VertexBufferUsage::Static);
		va->add_stream(vd2, sizeof(TileVertex));

		// Setup the static position of each vertex
		PosVertex *v = (PosVertex*)va->lock(0, 0, 4*_mw);
		int bx = 0;
		for(int x = 0; x<_mw; x++)
		{
			v->x = bx;
			v->y = 0;
			v++;

			v->x = bx+_tw;
			v->y = 0;
			v++;

			v->x = bx+_tw;
			v->y = _th;
			v++;

			v->x = bx;
			v->y = _th;
			v++;

			bx += _tw;
		}

		lay->_va[y] = va;
	}

	// Create the dirty flags array
	lay->_dirty = (bool*)malloc(_mh*sizeof(bool));
	for(int c = 0; c<_mh; c++)
		lay->_dirty[c] = true;

	#ifdef DBG
	Log::debug("Added tiles layer, tileset is %ix%i and holds %i tiles of %ix%i pixels", lay->_tx, lay->_ty, lay->_tx*lay->_ty, _tw, _th);
	#endif

	return lay->_index;
}

int TMBase::add_layer_int32(int32_t def)
{
	// Add an int32 layer
	TMLayer *lay = add_layer(LayerType::Int32);

	// Create the array
	int sz = _mw*_mh;
	lay->_i32 = (int32_t*)malloc(_mw*_mh*4);

	// Setup the default values
	int32_t *ptr = lay->_i32;
	for(int c = 0; c<sz; c++)
		*(ptr++) = def;

	return lay->_index;
}

int TMBase::add_layer_int64(int64_t def)
{
	// Add an int64 layer
	TMLayer *lay = add_layer(LayerType::Int64);

	// Create the array
	int sz = _mw*_mh;
	lay->_i64 = (int64_t*)malloc(_mw*_mh*8);

	// Setup the default values
	int64_t *ptr = lay->_i64;
	for(int c = 0; c<sz; c++)
		*(ptr++) = def;

	return lay->_index;
}

int TMBase::add_layer_str(const Str& def)
{
	// Add a string layer
	TMLayer *lay = add_layer(LayerType::Str);

	// Create the array
	int sz = _mw*_mh;
	lay->_str = (Str**)malloc(_mw*_mh*sizeof(Str*));

	// Create each string
	Str **ptr = lay->_str;
	for(int c = 0; c<sz; c++)
		*(ptr++) = new Str(def);

	return lay->_index;
}


void *TMBase::get_tile_base(int layer, int x, int y, bool set_dirty)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::get_tile(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::get_tile(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Tiles)
		E::BadLayerType("TileMap::get_tile(): Layer is not a tiles layer");

	// Mark the row dirty if needed
	if(set_dirty)
		lay->_dirty[y] = true;

	// Return the base address of the tile with the specified coordinates
	return ((uint8_t*)lay->_tiles) + (y*_mw+x)*_tsz;
}
	
void TMBase::draw_tile_layer(TMLayer *lay, int x1, int y1, int w, int h, int bx, int by)
{
	// Bind the shader and tileset texture
	lay->_ts->bind(0);
	_shad->bind();

	// Prepare the matrix
	Matrix mat = Matrix::identity();

	// Draw each row
	for(int y = 0; y<h; y++)
	{
		int yy = y1+y;
		VertexArray *va = lay->_va[yy];

		// Rebuild the vertices if dirty
		if(lay->_dirty[yy])
		{
			// Lock the VBO
			TileVertex *v = (TileVertex*)va->lock(1, 0, 4*_mw);

			// Get the first tile of the row
			uint8_t *ptr = (uint8_t*)get_tile_base(lay->_index, 0, yy, false);

			for(int x = 0; x<_mw; x++)
			{
				BaseTile *bt = (BaseTile*)ptr;
				ptr += _tsz;

				int tx = bt->tile % lay->_tx;
				int ty = bt->tile / lay->_tx;
				float u1 = (float)tx * lay->_uf;
				float v1 = (float)ty * lay->_vf;

				// Fill in the 4 vertices
				v->col = bt->col[0];
				v->u = u1;
				v->v = v1;
				v++;

				v->col = bt->col[1];
				v->u = u1+lay->_uf;
				v->v = v1;
				v++;

				v->col = bt->col[2];
				v->u = u1+lay->_uf;
				v->v = v1+lay->_vf;
				v++;

				v->col = bt->col[3];
				v->u = u1;
				v->v = v1+lay->_vf;
				v++;
			}

			lay->_dirty[yy] = false;
		}

		// Set the matrix
		mat._14 = bx - x1*_tw;
		mat._24 = by + y*_th;
		_shad->set_uniform("mat", mat);

		// Draw the tiles
		va->bind(_shad);
		Display::draw(x1*4, w*4);
	}
}

void TMBase::draw(const Rect& dest, int x, int y)
{
	// Calc the draw range
	int dx = dest.x;
	int dy = dest.y;
	int w = dest.w;
	int h = dest.h;

	int x1 = x/_tw;
	int x2 = (x+w-1)/_tw;
	int y1 = y/_th;
	int y2 = (y+h-1)/_th;

	if(x1<0)
	{
		dx += x1*-32;
		w += x1*32;
		x1 = 0;
	}
	if(x2>=_mw)
		x2 = _mw-1;

	if(y1<0)
	{
		dy += y1*-32;
		h += y1*32;
		y1 = 0;
	}
	if(y2>=_mh)
		y2 = _mh-1;

	// Abort if nothing to draw
	if(x1>x2 || y1>y2 || w<=0 || h<=0)
		return;

	// Prepare to draw
	Display::get_2d_camera().set();

	// Draw each layer
	for(int c = 0; c<_layers.size(); c++)
	{
		TMLayer *lay = _layers[c];

		switch(lay->_type)
		{
			case LayerType::Tiles:
				// Tiles layer
				draw_tile_layer(lay, x1, y1, x2-x1+1, y2-y1+1, dx-(x%_tw), dy-(y%_th));
				break;

			default:
				// Undrawn layer
				break;
		}
	}

	// Unbind everything
	Shader::unbind();
	VertexArray::unbind();
	Texture::unbind(0);
}


}

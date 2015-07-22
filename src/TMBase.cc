#include "pix5.h"
#include "data.h"
#include <GL/glew.h>

namespace PIX {


struct BaseTile
{
	int x, y;
	uint32_t tile;
	uint32_t col[4];
};

struct TileVertex
{
	float x, y;
	uint32_t col;
	float u, v;
};

class VirtualTileEntry
{
public:
	uint32_t tile;
	uint32_t ms;

	VirtualTileEntry(uint32_t tile, uint32_t ms)
	{
		this->tile = tile;
		this->ms = ms;
	}
};

class VirtualTile
{
public:
	
	List<VirtualTileEntry*> tiles;
	// All the tiles in the VT

	uint32_t tot_ms;
	// Total ms in the VT

	VirtualTile()
	{
		tot_ms = 0;
	}

	~VirtualTile()
	{
		tiles.clear_del();
	}
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

	_rect = Rect(100, 100, 320, 240);
	_x = 0;
	_y = 0;

	// Load the shader
	_shad = Shader::create_from_source("TileMapShader", (char*)data_sprite_vs, data_sprite_vs_len, (char*)data_sprite_fs, data_sprite_fs_len);
}

TMBase::~TMBase()
{
	_layers.clear_del();

	_vtiles.clear_del();

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

	// Create the vertex array
	VertexDef vd;
	vd.add(VertexComp::float2("pos", 0));
	vd.add(VertexComp::ubyte4("col", true, 8));
	vd.add(VertexComp::float2("texcoord", 12));

	lay->_vb = new VertexBuffer(vd, sizeof(TileVertex), VertexBufferUsage::Stream);

	#ifdef DBG
	Log::debug("Added tiles layer, tileset is %ix%i and holds %i tiles of %ix%i pixels", lay->_tx, lay->_ty, lay->_tx*lay->_ty, _tw, _th);
	#endif

	return lay->_index;
}
	
int TMBase::add_layer_sprites()
{
	// Add a sprites layer
	TMLayer *lay = add_layer(LayerType::Sprites);

	lay->_sprites = (MapSprite**)calloc(_mw*_mh, sizeof(MapSprite*));
	lay->_num_sprite = 0;

	// Create the vertex array
	VertexDef vd;
	vd.add(VertexComp::float2("pos", 0));
	vd.add(VertexComp::ubyte4("col", true, 8));
	vd.add(VertexComp::float2("texcoord", 12));

	lay->_vb = new VertexBuffer(vd, sizeof(TileVertex), VertexBufferUsage::Stream);

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


void *TMBase::get_tile_base(int layer, int x, int y)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::get_tile(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::get_tile(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Tiles)
		E::BadLayerType("TileMap::get_tile(): Layer is not a tiles layer");

	// Return the base address of the tile with the specified coordinates
	return ((uint8_t*)lay->_tiles) + (y*_mw+x)*_tsz;
}
	
int TMBase::get_vt_cur(uint32_t vti)
{
	VirtualTile *vt = _vtiles[vti];

	// Get the current timing
	uint32_t t = get_ms()%vt->tot_ms;

	// Find out which tile it's part of
	for(int c = 0; c<vt->tiles.size(); c++)
	{
		uint32_t ms = vt->tiles[c]->ms;

		if(t<ms)
			return c;
		else
			t -= ms;
	}

	return vt->tiles.size()-1;
}

void TMBase::set_rect(const Rect& r)
{
	_rect = r;
}

void TMBase::set_pos(int x, int y)
{
	_x = x;
	_y = y;
}

void TMBase::draw_tile_layer(TMLayer *lay, int x1, int y1, int w, int h, int bx, int by)
{
	// Bind the tileset texture
	lay->_ts->bind(0);

	// Prepare the matrix
	Matrix mat = Matrix::identity();
	mat._14 = bx;
	mat._24 = by;
	_shad->set_uniform("mat", mat);

	// Build the vertices
	TileVertex *v = (TileVertex*)lay->_vb->lock(0, w*h*4);

	int dy = 0;

	for(int y = 0; y<h; y++)
	{
		// Get the first tile of the row
		uint8_t *ptr = (uint8_t*)get_tile_base(lay->_index, x1, y1+y);

		int dx = 0;

		for(int x = 0; x<w; x++)
		{
			BaseTile *bt = (BaseTile*)ptr;
			ptr += _tsz;

			int tx, ty;

			// Handle virtual tiles
			if(bt->tile & 0x80000000)
			{
				uint32_t vti = bt->tile & 0x7FFFFFFF;
				if(vti<(uint32_t)_vtiles.size())
				{
					// Pick the proper sub-tile from the virtual tile
					uint32_t t = _vtiles[vti]->tiles[get_vt_cur(vti)]->tile;
					tx = t % lay->_tx;
					ty = t / lay->_tx;
				}
				else
				{
					tx = 0;
					ty = 0;
				}
			}
			else
			{
				tx = bt->tile % lay->_tx;
				ty = bt->tile / lay->_tx;
			}

			float u1 = (float)tx * lay->_uf;
			float v1 = (float)ty * lay->_vf;

			// Fill in the 4 vertices
			v->x = dx;
			v->y = dy;
			v->col = bt->col[0];
			v->u = u1;
			v->v = v1;
			v++;

			v->x = dx+_tw;
			v->y = dy;
			v->col = bt->col[1];
			v->u = u1+lay->_uf;
			v->v = v1;
			v++;

			v->x = dx+_tw;
			v->y = dy+_th;
			v->col = bt->col[2];
			v->u = u1+lay->_uf;
			v->v = v1+lay->_vf;
			v++;

			v->x = dx;
			v->y = dy+_th;
			v->col = bt->col[3];
			v->u = u1;
			v->v = v1+lay->_vf;
			v++;

			dx += _tw;
		}

		dy += _th;
	}
		
	// Draw the tiles
	lay->_vb->bind();
	lay->_vb->set_attribs(_shad);
	Display::draw(0, w*h*4);
}

void TMBase::draw_sprite_layer(TMLayer *lay, int x1, int y1, int w, int h, int dx, int dy)
{
	// Build the vertices
	TileVertex *v = (TileVertex*)lay->_vb->lock(0, lay->_num_sprite*4);

	int count = 0;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
			
	for(int y = 0; y<h; y++)
	{
		for(int x = 0; x<w; x++)
		{
			// Do all the visible sprites for this tile
			for(MapSprite *ms = lay->_sprites[(y1+y)*_mw+x1+x]; ms; ms = ms->_next)
				if(ms->visible)
				{
					// Recalc the matrix ?
					if(ms->_mat_dirty)
					{
						// Build the matrix for this sprite
						glLoadIdentity();

						// Rotation
						if(ms->_angle)
							glRotatef(ms->_angle, 0, 0, 1.0f);

						// Scaling
						if(ms->_hscale!=1.0f || ms->_vscale!=1.0f)
							glScalef(ms->_hscale, ms->_vscale, 1.0f);

						// Origin
						if(ms->_ox || ms->_oy)
							glTranslatef(-ms->_ox, -ms->_oy, 0);

						// Save the matrix
						ms->_mat = Matrix::get_modelview();

						ms->_mat_dirty = false;
					}

					float u1, v1, u2, v2;
					int ww, hh;

					// Get dimensions and texture coordinates
					if(ms->_tex)
					{
						ww = ms->_w;
						hh = ms->_h;
						u1 = ms->_u1;
						v1 = ms->_v1;
						u2 = ms->_u2;
						v2 = ms->_v2;
					}
					else
					{
						AnimFrame *frm = ms->_anim->get_frame(ms->_anim_state, false);
						ww = frm->w;
						hh = frm->h;
						u1 = frm->u1;
						v1 = frm->v1;
						u2 = frm->u2;
						v2 = frm->v2;
					}

					float fx = (float)dx + (ms->_x * (float)_tw) - (float)_x + (float)_tw/2.0f;
					float fy = (float)dy + (ms->_y * (float)_th) - (float)_y + (float)_th/2.0f;

					// Setup the vertices
					v->x = fx;
					v->y = fy;
					v->col = ms->col;
					v->u = u1;
					v->v = v1;
					v++;

					v->x = fx+ww;
					v->y = fy;
					v->col = ms->col;
					v->u = u2;
					v->v = v1;
					v++;

					v->x = fx+ww;
					v->y = fy+hh;
					v->col = ms->col;
					v->u = u2;
					v->v = v2;
					v++;

					v->x = fx;
					v->y = fy+hh;
					v->col = ms->col;
					v->u = u1;
					v->v = v2;
					v++;

					count++;
				}
		}
	}
	
	glPopMatrix();
		
	// Draw the sprites
	if(count)
	{
		lay->_vb->bind();
		lay->_vb->set_attribs(_shad);

		int c = 0;
		Texture *last_tex = NULL;
	
		for(int y = 0; y<h; y++)
		{
			for(int x = 0; x<w; x++)
			{
				// Do all the visible sprites for this tile
				for(MapSprite *ms = lay->_sprites[(y1+y)*_mw+x1+x]; ms; ms = ms->_next)
					if(ms->visible)
					{
						// Set the matrix
						_shad->set_uniform("mat", ms->_mat);

						// Set the texture
						Texture * tex = ms->_tex ? ms->_tex : ms->_anim->get_frame(ms->_anim_state)->tex;
						if(tex!=last_tex)
						{
							tex->bind(0);
							last_tex = tex;
						}

						// Draw it
						Display::draw(c, 4);
						c += 4;
					}
			}
		}
	}
}

void TMBase::draw()
{
	// Calc the draw range
	int dx = _rect.x;
	int dy = _rect.y;
	int w = _rect.w;
	int h = _rect.h;

	int x1 = _x/_tw;
	int x2 = (_x+w-1)/_tw;
	int y1 = _y/_th;
	int y2 = (_y+h-1)/_th;

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

	// For sprites, always check a few tiles around the region
	int sx1 = Math::max(x1-8, 0);
	int sy1 = Math::max(y1-8, 0);

	int sw = (x2-sx1)+8;
	int sh = (y2-sy1)+8;

	if(sx1+sw>_mw)
		sw = _mw-sx1;
	if(sy1+sh>_mh)
		sh = _mh-sy1;

	// Prepare to draw
	Display::get_2d_camera().set();

	// Make sure no VA is bound
	VertexArray::unbind();
	
	// Bind the shader
	_shad->bind();

	// Draw each layer
	for(int c = 0; c<_layers.size(); c++)
	{
		TMLayer *lay = _layers[c];

		switch(lay->_type)
		{
			case LayerType::Tiles:
				// Tiles layer
				draw_tile_layer(lay, x1, y1, x2-x1+1, y2-y1+1, dx-(_x%_tw), dy-(_y%_th));
				break;

			case LayerType::Sprites:
				// Sprites layer
				draw_sprite_layer(lay, sx1, sy1, sw, sh, _rect.x, _rect.y);
				break;

			default:
				// Undrawn layer
				break;
		}
	}

	// Unbind everything
	Shader::unbind();
	VertexBuffer::unbind();
	VertexBuffer::disable_attribs();
	Texture::unbind(0);
}

int32_t TMBase::get_int32(int layer, int x, int y)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::get_int32(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::get_int32(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int32)
		E::BadLayerType("TileMap::get_int32(): Layer is not an Int32 layer");

	return get_int32_fast(layer, x, y);
}

int64_t TMBase::get_int64(int layer, int x, int y)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::get_int64(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::get_int64(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int64)
		E::BadLayerType("TileMap::get_int64(): Layer is not an Int64 layer");

	return get_int64_fast(layer, x, y);
}
	
Str TMBase::get_str(int layer, int x, int y)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::get_str(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::get_str(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int64)
		E::BadLayerType("TileMap::get_str(): Layer is not a Str layer");

	return get_str_fast(layer, x, y);
}

void TMBase::set_int32(int layer, int x, int y, int32_t val)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::set_int32(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::set_int32(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int32)
		E::BadLayerType("TileMap::set_int32(): Layer is not an Int32 layer");

	set_int32_fast(layer, x, y, val);
}

void TMBase::set_int64(int layer, int x, int y, int64_t val)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::set_int64(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::set_int64(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int64)
		E::BadLayerType("TileMap::set_int64(): Layer is not an Int64 layer");

	set_int64_fast(layer, x, y, val);
}

void TMBase::set_str(int layer, int x, int y, const Str& val)
{
	// Validate
	if(layer<0 || layer>=_layers.size())
		E::BadTileCoord("TileMap::set_str(): Invalid layer index");
	if(x<0 || x>=_mw || y<0 || y>=_mh)
		E::BadTileCoord("TileMap::set_str(): Coordinates out of bounds");

	TMLayer *lay = _layers[layer];
	if(lay->_type!=LayerType::Int64)
		E::BadLayerType("TileMap::set_str(): Layer is not a Str layer");

	set_str_fast(layer, x, y, val);
}
	
uint32_t TMBase::add_virt_tile(uint32_t vt, uint32_t t, uint32_t ms)
{
	// Validate
	if(t&0x80000000)
		E::BadTileIndex("add_virt_tile(): The provided sub-tile cannot be a virtual tile");

	if(vt && !(vt&0x80000000))
		E::BadTileIndex("add_virt_tile(): Invalid virtual tile index (1)");

	// Create a new virtual tile?
	if(!vt)
		vt = _vtiles.add(new VirtualTile()) | 0x80000000;

	// Get the VT index
	uint32_t vti = vt & 0x7FFFFFFF;

	if(vti>=(uint32_t)_vtiles.size())
		E::BadTileIndex("add_virt_tile(): Invalid virtual tile index (2)");

	// Add the sub-tile
	_vtiles[vti]->tiles.add(new VirtualTileEntry(t, ms));
	_vtiles[vti]->tot_ms += ms;

	return vt;
}

MapSprite *TMBase::new_sprite(int layer, Texture *tex, float x, float y, int w, int h)
{
	// Create the sprite
	MapSprite *ms = new MapSprite(this, layer);

	ms->_tex = tex;
	ms->_x = x;
	ms->_y = y;
	ms->_w = (w==-1) ? tex->width() : w;
	ms->_h = (h==-1) ? tex->height() : h;

	// Add it
	add_sprite_to_tile(ms);

	return ms;
}

MapSprite *TMBase::new_sprite(int layer, Anim *anim, float x, float y)
{
	// Create the sprite
	MapSprite *ms = new MapSprite(this, layer);

	ms->_anim = anim;
	ms->_x = x;
	ms->_y = y;

	AnimFrame *frm = anim->get_frame(ms->_anim_state, false);
	ms->_w = frm->w;
	ms->_h = frm->h;

	// Add it
	add_sprite_to_tile(ms);

	return ms;
}

void TMBase::get_ipos(MapSprite *ms, int *ix, int *iy)
{
	*ix = (int)ms->_x;
	*iy = (int)ms->_y;

	// Clip
	if(*ix<0)
		*ix = 0;
	if(*ix>=_mw)
		*ix = _mw-1;

	if(*iy<0)
		*iy = 0;
	if(*iy>=_mh)
		*iy = _mh-1;
}

void TMBase::add_sprite_to_tile(MapSprite *ms)
{
	// Add the sprite to the tile's linked list
	TMLayer *lay = _layers[ms->_layer];

	int ix, iy;
	get_ipos(ms, &ix, &iy);

	// Add it
	int i = iy*_mw+ix;

	ms->_next = lay->_sprites[i];
	lay->_sprites[i] = ms;

	lay->_num_sprite++;
}

void TMBase::remove_sprite_from_tile(MapSprite *ms)
{
	// Remove the sprite from the tile's linked list
	TMLayer *lay = _layers[ms->_layer];

	int ix, iy;
	get_ipos(ms, &ix, &iy);

	// Remove it
	int i = iy*_mw+ix;

	if(lay->_sprites[i]==ms)
		lay->_sprites[i] = ms->_next;
	else
	{
		for(MapSprite *run = lay->_sprites[i]; run->_next; run = run->_next)
			if(run->_next==ms)
			{
				run->_next = ms->_next;
				break;
			}
	}

	lay->_num_sprite--;
}







}

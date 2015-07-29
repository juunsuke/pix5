#pragma once


class TMBase
{
	int _tw, _th;
	// Tile dimensions

	int _tsz;
	// Size of the tile structure

	int _mw, _mh;
	// Map dimensions, in tiles

	List<TMLayer*> _layers;
	// All the layers

	Shader *_shad;
	// The tilemap shader

	List<class VirtualTile*> _vtiles;
	// Virtual tiles

	Rect _rect;
	// Display rect

	int _x, _y;
	// Draw position


	TMLayer *add_layer(LayerType::Type type);
	// Create and insert a new layer

	void draw_tile_layer(TMLayer *lay, int x1, int y1, int w, int h, int bx, int by);
	void draw_sprite_layer(TMLayer *lay, int x1, int y1, int w, int h, int dx, int dy);
	void draw_int32_layer(TMLayer *lay, int x1, int y1, int w, int h, int bx, int by);

	int get_vt_cur(uint32_t vti);
	// Get the current VT sub-tile index for the given VT index based on time

	void add_sprite_to_tile(MapSprite *ms);
	void remove_sprite_from_tile(MapSprite *ms);
	// Add/remove the sprite to/from its tile

	void get_ipos(MapSprite *ms, int *ix, int *iy);
	// Calc the map position for the sprite

	void create_vb(TMLayer *lay);
	// Create the VB for a layer

	friend class MapSprite;


protected:

	TMBase(int mw, int mh, int tw, int th, int tsz);

	void *get_tile_base(int layer, int x, int y);
	// Get the base address of a tile

public:

	virtual ~TMBase();

	inline int width() { return _mw; }
	inline int height() { return _mh; }
	// REturn the map timensions

	inline int tile_width() { return _tw; }
	inline int tile_height() { return _th; }
	// Retrieve the tile dimensions

	int add_layer_tiles(Texture *ts);
	// Add a tiles layer
	// The given tileset will be used for that layer, and must be divisible
	// by the tile dimensions in both sides
	// The tile map doesn't take possession of the texture nor copy it, thus
	// the provided texture must be kept and freed when done with the map
	// Returns the layer index

	int add_layer_sprites();
	// Add a sprites layer
	// Returns the layer index

	int add_layer_int32(int32_t def);
	int add_layer_int64(int64_t def);
	// Add a 32/64-bits integer layer
	// 'def' is the default value to set all tiles to
	// Returns the layer index

	int add_layer_str(const Str& def);
	// Add a string layer
	// 'def' is the default value to set all tiles to
	// Returns the layer index

	inline Rect rect() { return _rect; }
	// Get a copy of the destination rectangle used for rendering

	void set_rect(const Rect& r);
	// Set the destination rectangle, which will be used to draw the map
	// This will cause the vertex buffers to be re-created on the next render,
	// so this shouldn't be done at every frame, but when the display area actually changes

	inline int x() { return _x; }
	inline int y() { return _y; }
	// Get the draw position

	void set_pos(int x, int y);
	// Set the draw position of the map, in map-wise pixels
	// The pixel represented by (x,y) will be located on the top-left of the destination rectangle

	void enable_layer(int layer, bool enable = true);
	inline void disable_layer(int layer) { enable_layer(layer, false); }
	// Enable/disable a layer
	// Disabled layers will not be drawn when draw() is called

	void draw(int first, int last);
	void draw();
	// Draw the tiled map
	// This will leave any shader, VB/VA, and texture undbound
	// The camera will be set to the 2D camera

	int32_t get_int32(int layer, int x, int y);
	int64_t get_int64(int layer, int x, int y);
	Str get_str(int layer, int x, int y);
	void set_int32(int layer, int x, int y, int32_t val);
	void set_int64(int layer, int x, int y, int64_t val);
	void set_str(int layer, int x, int y, const Str& val);
	// Get/set the value of an Int32/Int64/Str layer for the given tile
	// Throws an error if the layer is not an Int32/Int64/Str layer, or if
	// the coordinates are out of range

	uint32_t add_virt_tile(uint32_t vt, uint32_t t, uint32_t ms);
	// Add a sub-tile to a virtual tile, or create a new virtual tile
	// If 'vt' is 0, create a new virtual tile and return its index
	// If 'vt' is not 0, it must be an already created virtual tile
	// 't' is a regular, non-virtual, tile index to add
	// 'ms' is the number of milliseconds the tile 't' should be visible
	// before switching to the next sub-tile

	MapSprite *new_sprite(int layer, Texture *tex, float x, float y, int w = -1, int h = -1);
	MapSprite *new_sprite(int layer, Anim *anim, float x, float y);
	// Create a new sprite and add it to the map
	// The positions are given in fractional tile coordinates, with the integer
	// value corresponding to the center of the tile
	
	

	//
	// Fast inline getters/setters for value layers
	// Absolutely no layer validation nor bounds checking is performed, as to access the
	// data as fast as possible
	// These can be used if you are 100% certain the provided values are valid and within bounds
	// In most cases, the regular access functions should be used
	//
	
	inline int32_t get_int32_fast(int layer, int x, int y)				{ return _layers[layer]->_i32[y*_mw+x];		}
	inline int32_t get_int64_fast(int layer, int x, int y)				{ return _layers[layer]->_i64[y*_mw+x];		}
	inline Str get_str_fast(int layer, int x, int y)					{ return *_layers[layer]->_str[y*_mw+x];	}
	inline void set_int32_fast(int layer, int x, int y, int32_t val)	{ _layers[layer]->_i32[y*_mw+x] = val;		}
	inline void set_int64_fast(int layer, int x, int y, int64_t val)	{ _layers[layer]->_i64[y*_mw+x] = val;		}
	inline void set_str_fast(int layer, int x, int y, const Str& val)	{ *_layers[layer]->_str[y*_mw+x] = val;		}
};



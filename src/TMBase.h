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


	TMLayer *add_layer(LayerType::Type type);
	// Create and insert a new layer

	void draw_tile_layer(TMLayer *lay, int x1, int y1, int w, int h, int bx, int by);

protected:

	TMBase(int mw, int mh, int tw, int th, int tsz);

	void *get_tile_base(int layer, int x, int y, bool set_dirty);
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

	int add_layer_int32(int32_t def);
	int add_layer_int64(int64_t def);
	// Add a 32/64-bits integer layer
	// 'def' is the default value to set all tiles to
	// Returns the layer index

	int add_layer_str(const Str& def);
	// Add a string layer
	// 'def' is the default value to set all tiles to
	// Returns the layer index

	void draw(const Rect& dest, int x, int y);
	// Draw the tiled map in such a way that the 'dest' rectangle is filled with tiles
	// Drawing might (and will) overlap outside of the bounds defined by 'dest'
	// (x,y) represents the map coordinate, in pixels, that should be at (dest.x,dest.y)
	// This will leave any shader, VB/VA, and texture undbound
	// The camera will be set to the 2D camera
};



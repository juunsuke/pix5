#pragma once


class TileMap
{
	int _mw, _mh;
	// Map size

	int _tw, _th;
	// Tile size

	List<TMLayer*> _layers;
	// All the layers



	void reset();
	void release();

public:

	TileMap();
	TileMap(int mw, int mh, int tw, int th);
	// See reset() for info

	~TileMap();

	inline int width() { return _mw; }
	inline int height() { return _mh; }
	inline int tile_width() { return _tw; }
	inline int tile_height() { return _th; }
	// Get info about the map

	void reset(int mw, int mh, int tw, int th);
	// Reset the tile map, releasing all layers and data and basically creating a new map
	// mw,mh: Map size, in tiles
	// tw,th: Individual tiles size, in pixels

	int add_layer(TMLayer *layer);
	// Add a custom layer to the tilemap
	// Returns the layer index

	int add_tile_layer(Texture *tex);
	// Add a tile layer to the tilemap
	// Returns the layer index
	// The texture holds the tileset to be used, and its dimensions should be divisible
	// by the tile size in both dimensions
	// The texture will not be deleted

	uint16_t get_tile(int layer, int x, int y);
	// Get the tile index at the given coordinates for the given layer
	// All values are validated, and 0 is returned if any of them is invalid
	// That is, if the layer index is out of range, or does no refer to a tiles layer, or
	// if coordinates are out of range

	void set_tile(int layer, int x, int y, uint16_t t);
	// Set the tile index at the given coordinates for the given layer
	// All values are validated, and nothing happens if any of them is invalid
	// That is, if the layer index is out of range, or does no refer to a tiles layer, or
	// if coordinates are out of range


	void draw(int mx, int my, const Rect& dest);
	void draw(int mx, int my);
	// Draw the tile map
	// 'dest' represents an area on screen that should be completely filled by tiles
	// If ommited, the whole screen is used
	// No clipping is performed, tiles may and will overlap to make sure the area is filled by tiles
	// (mx,my) represents the map pixel that will be located at dest.x,dest.y
};


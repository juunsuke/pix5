#pragma once


class TMTileLayer: public TMLayer
{
	Texture *_tex;
	// Tilset texture

	int _tpr;
	// Tiles per row in the texture

	Buffer<uint16_t> _data;
	// Raw tiles data


	friend class TileMap;

public:

	TMTileLayer(int mw, int mh, int tw, int th, Texture *tex);
	~TMTileLayer();


	void draw(int dx, int dy, int tx1, int ty1, int tw, int th);
};


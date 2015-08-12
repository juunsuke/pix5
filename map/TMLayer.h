#pragma once


class TMLayer
{
	int _mw, _mh;
	// Map size

	int _tw, _th;
	// Tile size

	bool _visible;
	// Wether the layer is visible


	friend class TileMap;


protected:
	

	TMLayer(int mw, int mh, int tw, int th)
	{
		ASSERT(mw>0 && mh>0, "TMLayer::TMLayer(): Invalid map size")
		ASSERT(tw>0 && th>0, "TMLayer::TMLayer(): Invalid tile size")

		_mw = mw;
		_mh = mh;
		_tw = tw;
		_th = th;

		_visible = true;
	}


public:

	virtual ~TMLayer()
	{
	}

	inline int width() { return _mw; }
	inline int height() { return _mh; }
	inline int tile_width() { return _tw; }
	inline int tile_height() { return _th; }
	// Get info about the layer

	inline bool is_visible() { return _visible; }
	inline void set_visible(bool vis) { _visible = vis; }
	inline void show() { set_visible(true); }
	inline void hide() { set_visible(false); }
	// Get/set visibility


	virtual void draw(int dx, int dy, int tx1, int ty1, int tw, int th) {};
	// Draw the layer

};


#pragma once


#define TILE_BASE \
	int x, y; \
	uint32_t tile; \
	uint32_t col[4];


template<class T> class TileMap: public TMBase
{
public:

	TileMap(int mw, int mh, int tw, int th) : TMBase(mw, mh, tw, th, sizeof(T))
	{
		#ifdef DBG
		T *t = (T*)NULL;
		intptr_t a1 = (intptr_t)&t->x;
		intptr_t a2 = (intptr_t)&t->y;
		intptr_t a3 = (intptr_t)&t->tile;
		intptr_t a4 = (intptr_t)&t->col;

		if(a1!=0 || a2!=4 || a3!=8 || a4!=12 || sizeof(t->col)!=16)
			E::Assert("TileMap<%s>::TileMap(): The provided tile structure definition must begin with TILE_BASE", demangle(typeid(T).name()).ptr());
		#endif
	}

	~TileMap()
	{
	}

	inline T *get_tile(int layer, int x, int y, bool set_dirty = false)
	{
		// Get the given tile
		return (T*)get_tile_base(layer, x, y, set_dirty);
	}
};


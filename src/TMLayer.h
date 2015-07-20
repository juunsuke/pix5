#pragma once


namespace LayerType
{
	enum Type
	{
		Tiles,
		Sprites,
		Int32,
		Int64,
		Str,
	};
}


class TMLayer
{
public:

	int _index;
	// Layer index

	LayerType::Type _type;
	// Layer type

	int _w, _h;
	// Layer size, which is always the same as the map size

	VertexBuffer *_vb;
	// The array's VB, if any

	union
	{
		struct
		{
			void *_tiles;
			// Tiles array, for tile layers

			Texture *_ts;
			// Tileset texture

			int _tx, _ty;
			// Number of tiles in either dimension

			float _uf, _vf;
			// U and V factors for the tileset
		};

		struct
		{
			MapSprite **_sprites;
			// Map sprites array, for sprite layers

			int _num_sprite;
			// Total number of current sprites
		};

		int32_t *_i32;
		int64_t *_i64;
		// Integer arrays, for Int32/Int64 layers

		Str **_str;
		// String pointer array, for string layers
	};


	TMLayer(LayerType::Type type, int w, int h);
	~TMLayer();
};



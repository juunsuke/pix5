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

			VertexArray **_va;
			// Vertex arrays, one per row

			bool *_dirty;
			// Dirty flags, one per row
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



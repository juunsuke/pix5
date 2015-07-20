#pragma once

class MapSprite
{
	class TMBase *_tm;
	// The tilemap we're attached it

	int _layer;
	// The layer index we're part of

	float _x, _y;
	// Current position, in tiles

	Texture *_tex;
	// Texture, for tex-based sprites

	Anim *_anim;
	// Anim, for anim-based sprites

	int _w, _h;
	// Sprite size, either forced or taken from anim

	Matrix _mat;
	// The sprite's matrix, used by the renderer

	float _u1, _v1, _u2, _v2;
	// Texture coordinates


	MapSprite *_next;
	// Next sprite, for linked list


	MapSprite(class TMBase *tm, int layer);
	// Private constructor, use TMBase::new_sprite() to create a new map sprite

	friend class TMBase;
	friend class TMLayer;

public:

	int ox, oy;
	// Origin coordinates

	float angle;
	// Rotation angle

	float hscale, vscale;
	// Horizontal/vertical scale

	Color col;
	// Sprite color

	bool visible;
	// Wether the map sprite is visible or not



	~MapSprite();


	inline float x() { return _x; }
	inline float y() { return _y; }
	// Get the current position, expressed in fractional tile values

	void set_pos(float x, float y);
	// Change the sprite position
	// The position is expressed in fractional values, with integer values
	// representing the center of the given tile

	inline void show() { visible = true; }
	inline void hide() { visible = false; }
	// Show/hide the sprite

	void set_tex(Texture *tex, bool resize = true, float u1 = 0, float v1 = 0, float u2 = 1, float v2 = 1);
	// Change the current sprite texture

	void set_anim(Anim *anim);
	// Change the current sprite anim
};


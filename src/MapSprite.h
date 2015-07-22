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

	AnimState _anim_state;

	int _w, _h;
	// Sprite size, either forced or taken from anim

	Matrix _mat;
	// The sprite's matrix, used by the renderer

	float _u1, _v1, _u2, _v2;
	// Texture coordinates

	int _ox, _oy;
	// Origin coordinates

	float _angle;
	// Rotation angle

	float _hscale, _vscale;
	// Horizontal/vertical scale

	bool _mat_dirty;
	// Dirty matrix flag


	MapSprite *_next;
	// Next sprite, for linked list


	MapSprite(class TMBase *tm, int layer);
	// Private constructor, use TMBase::new_sprite() to create a new map sprite

	friend class TMBase;
	friend class TMLayer;

public:


	Color col;
	// Sprite color

	bool visible;
	// Wether the map sprite is visible or not



	~MapSprite();


	inline float x() { return _x; }
	inline float y() { return _y; }
	// Get the current position, expressed in fractional tile values
	
	inline int ox() const			{ return _ox; }
	inline int oy() const			{ return _oy; }
	inline float angle() const		{ return _angle; }
	inline float hscale() const		{ return _hscale; }
	inline float vscale() const		{ return _vscale; }
	// Get the map sprite parameters

	void set_pos(float x, float y);
	// Change the sprite position
	// The position is expressed in fractional values, with integer values
	// representing the center of the given tile

	inline void set_origin(int ox, int oy)
	{
		if(ox!=_ox || oy!=_oy)
		{
			_ox = ox;
			_oy = oy;
			_mat_dirty = true;
		}
	}

	inline void set_angle(float a)
	{
		if(a!=_angle)
		{
			_angle = a;
			_mat_dirty = true;
		}
	}

	inline void set_hscale(float hscale)
	{
		if(hscale!=_hscale)
		{
			_hscale = hscale;
			_mat_dirty = true;
		}
	}

	inline void set_vscale(float vscale)
	{
		if(vscale!=_vscale)
		{
			_vscale = vscale;
			_mat_dirty = true;
		}
	}

	inline void set_scale(float scale)
	{
		set_hscale(scale);
		set_vscale(scale);
	}

	inline void show() { visible = true; }
	inline void hide() { visible = false; }
	// Show/hide the sprite

	void set_tex(Texture *tex, bool resize = true, float u1 = 0, float v1 = 0, float u2 = 1, float v2 = 1);
	// Change the current sprite texture

	void set_anim(Anim *anim);
	// Change the current sprite anim

	inline void set_set(const Str& name, int frame = 0)
	{
		// Change the bound anim's set
		ASSERT(_anim, "MapSprite::set_set(): The sprite is not Anim-based")
		_anim->set_set(_anim_state, name, frame);
	}

	inline void change_set(const Str& name, int frame = 0)
	{
		// Change the bound anim's set
		ASSERT(_anim, "MapSprite::change_set(): The sprite is not Anim-based")
		_anim->change_set(_anim_state, name, frame);
	}
};


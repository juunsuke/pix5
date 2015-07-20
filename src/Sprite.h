#pragma once


class Sprite
{
	class SpriteSet *_ss;
	// The sprite set we're part of

	int _x, _y;
	// Position

	int _z;
	// Z-order

	int _w, _h;
	// Sprite size

	int _ox, _oy;
	// Origin position

	float _angle;
	// Rotation around the origin

	float _hscale, _vscale;
	// Horizontal and vertical scaling, based on the origin

	bool _visible;
	// Visibility flag

	Texture *_tex;
	// The sprite's current texture

	float _u1, _v1, _u2, _v2;
	// Area of the texture to use for the sprite

	Color _col;
	// Tint color

	Matrix _mat;
	// The current matrix

	Anim *_anim;
	AnimState _anim_state;
	// The sprite anim and its state, if any



	bool _mat_dirty, _vtx_dirty;
	// Various dirty flags, used by SpriteSet
	
	
	Sprite *_prev, *_next;
	// Previous/next link, for the hidden sprite list


	Sprite(class SpriteSet *ss);
	// Private constructor, create sprites using SpriteSet::new_sprite()

	friend class SpriteSet;

public:

	~Sprite();

	int comp(Sprite *o) const
	{
		// Compare sprites' Z values, used for being in a sorted list
		if(_z==o->_z)
			return 0;

		if(_z<o->_z)
			return -1;

		return 1;
	}



	//
	// Functions to retrieve the sprite's data
	//

	inline int x() const			{ return _x; }
	inline int y() const			{ return _y; }
	inline int z() const			{ return _z; }
	inline int w() const			{ return _w; }
	inline int h() const			{ return _h; }
	inline int ox() const			{ return _ox; }
	inline int oy() const			{ return _oy; }
	inline float angle() const		{ return _angle; }
	inline float hscale() const		{ return _hscale; }
	inline float vscale() const		{ return _vscale; }
	
	inline bool visible() const		{ return _visible; }

	inline Texture *tex() const		{ return _tex; }
	inline float u1() const			{ return _u1; }
	inline float v1() const			{ return _v1; }
	inline float u2() const			{ return _u2; }
	inline float v2() const			{ return _v2; }

	inline Anim *anim() const		{ return _anim; }

	inline Color col() const		{ return _col; }


	//
	// Functions to modify the sprite's data
	//

	inline void set_pos(int x, int y)
	{
		if(x!=_x || y!=_y)
		{
			_x = x;
			_y = y;
			_mat_dirty = true;
		}
	}

	void set_z(int z);
	// Change the Z order

	inline void set_size(int w, int h)
	{
		ASSERT(!_anim, "Sprite::set_size() should not be called for Anim-base sprites")

		if(w!=_w || h!=_h)
		{
			_w = w;
			_h = h;
			_mat_dirty = true;
		}
	}

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

	void set_visible(bool vis);

	inline void show() { set_visible(true); }
	inline void hide() { set_visible(false); }

	void set_tex(Texture *tex, bool resize = true, float u1 = 0, float v1 = 0, float u2 = 1, float v2 = 1);
	// Change the sprite's texture
	// If the sprite was anim-based, the anim will be dropped

	void set_anim(Anim *anim);
	// Change the sprite's anim
	// If the sprite was texture-based, the texture will be dropped

	inline void set_color(const Color& col)
	{
		if(_col!=col)
		{
			_col = col;
			_vtx_dirty = true;
		}
	}

	inline void set_set(const Str& name, int frame = 0)
	{
		// Change the bound anim's set
		ASSERT(_anim, "Sprite::set_set(): The sprite is not Anim-based")
		_anim->set_set(_anim_state, name, frame);
	}

	inline void change_set(const Str& name, int frame = 0)
	{
		// Change the bound anim's set
		ASSERT(_anim, "Sprite::change_set(): The sprite is not Anim-based")
		_anim->change_set(_anim_state, name, frame);
	}

	void calc_matrix();
	// Recalculate the sprite's matrix
};


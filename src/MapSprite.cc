#include "pix5.h"

namespace PIX {



MapSprite::MapSprite(TMBase *tm, int layer)
{
	_tm = tm;
	_layer = layer;
	_x = 0;
	_y = 0;
	_tex = NULL;
	_anim = NULL;
	_w = 0;
	_h = 0;

	ox = 0;
	oy = 0;
	angle = 0;
	hscale = 1.0f;
	vscale = 1.0f;

	col = Color(1,1,1,1);
	visible = true;

	_next = NULL;
}

MapSprite::~MapSprite()
{
	_tm->remove_sprite_from_tile(this);
}

void MapSprite::set_pos(float x, float y)
{
	// Check if the sprite changes tiles
	int ix = (int)x;
	int iy = (int)y;
	int ox = (int)_x;
	int oy = (int)_y;

	if(ix!=ox || iy!=oy)
	{
		// Yes, remove it from the current tile
	}
}

void MapSprite::set_tex(Texture *tex, bool resize, float u1, float v1, float u2, float v2)
{
	// Set the texture
	_tex = tex;
	_anim = NULL;

	if(resize)
	{
		_w = tex->width();
		_h = tex->height();
	}

	_u1 = u1;
	_v1 = v1;
	_u2 = u2;
	_v2 = v2;
}

void MapSprite::set_anim(Anim *anim)
{
	// Set the animation
	_anim = anim;
	_tex = NULL;
}



}

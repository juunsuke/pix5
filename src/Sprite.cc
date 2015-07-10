#include "pix5.h"

namespace PIX {

	
Sprite::Sprite(SpriteSet *ss)
{
	_ss = ss;

	_x = 0;
	_y = 0;
	_z = 0;
	_ox = 0;
	_oy = 0;
	_angle = 0;
	_hscale = 1;
	_vscale = 1;

	_visible = true;

	_tex = NULL;
	_u1 = 0;
	_v1 = 0;
	_u2 = 1;
	_v2 = 1;

	_col = Color(1,1,1,1);

	_mat_dirty = true;
	_vtx_dirty = true;

	_prev = NULL;
	_next = NULL;
}

Sprite::~Sprite()
{
}

void Sprite::set_z(int z)
{
	// Skip if no change
	if(z==_z)
		return;

	// Let the spriteset handle the change
	_ss->reorder_sprite(this, z);
}

void Sprite::set_visible(bool vis)
{
	// Skip if no change
	if(vis==_visible)
		return;

	_visible = vis;

	// Tell the sprite set to show/hide the sprite
	if(vis)
		_ss->show_sprite(this);
	else
		_ss->hide_sprite(this);
}


}


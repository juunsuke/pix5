#include "pix5.h"
#include "data.h"
#include <GL/glew.h>

namespace PIX {


typedef struct PosVertex
{
	float x, y;
} PosVertex;

typedef struct SpriteVertex
{
	uint32_t col;
	float u, v;
} SpriteVertex;


static Shader *_shad = NULL;
// The sprite shader


void sprite_init()
{
	// Load the sprite shader
	_shad = Shader::create_from_source("SpriteShader", (char*)data_sprite_vs, data_sprite_vs_len, (char*)data_sprite_fs, data_sprite_fs_len);
}

void sprite_done()
{
	// Delete the sprite shader
	if(_shad)
	{
		delete _shad;
		_shad = NULL;
	}
}

	
SpriteSet::SpriteSet()
{
	// Define the vertex format streams
	VertexDef vd;
	vd.add(VertexComp::float2("pos", 0));
	_va.add_stream(vd, sizeof(PosVertex), VertexBufferUsage::Static);

	vd.clear();
	vd.add(VertexComp::ubyte4("col", true, 0));
	vd.add(VertexComp::float2("texcoord", 4));
	_va.add_stream(vd, sizeof(SpriteVertex));

	// Create the first/last dummy sprites, for the hidden doubly-linked-list
	_hid_first = new Sprite(this);
	_hid_last = new Sprite(this);

	_hid_first->_next = _hid_last;
	_hid_last->_prev = _hid_first;

	_hid_first->_visible = false;
	_hid_last->_visible = false;

	_va_dirty = true;

	_max_size = 0;
}

SpriteSet::~SpriteSet()
{
	// Clear everything
	// Since Sprite has a private destructor, we must delete them manually here
	while(_vis_sprites.size())
		delete _vis_sprites[0];

	_vis_sprites.clear_nodel();

	delete _hid_first;
	delete _hid_last;
}

void SpriteSet::show_sprite(Sprite *s)
{
	// Go from hidden to visible
	del_hidden(s);
	add_visible(s);
}

void SpriteSet::hide_sprite(Sprite *s)
{
	// Go from visible to hidden
	del_visible(s);
	add_hidden(s);
}

void SpriteSet::reorder_sprite(Sprite *s, int new_z)
{
	// For hidden sprites, simply do the change
	if(!s->_visible)
	{
		s->_z = new_z;
		return;
	}

	// Visible sprite
	// Remove and re-add the sprite, after changing its Z order
	del_visible(s);
	s->_z = new_z;
	add_visible(s);
}
	
void SpriteSet::add_hidden(Sprite *s)
{
	// Add a sprite to the hidden sprites linked list
	s->_prev = _hid_first;
	s->_next = _hid_first->_next;

	s->_prev->_next = s;
	s->_next->_prev = s;
}

void SpriteSet::del_hidden(Sprite *s)
{
	// Remove a sprite from the hidden linked list
	if(!s->_prev || !s->_next)
		// First and last
		return;

	s->_prev->_next = s->_next;
	s->_next->_prev = s->_prev;

	s->_prev = NULL;
	s->_next = NULL;
}
	
void SpriteSet::add_visible(Sprite *s)
{
	// Add a sprite to the visible list
	_vis_sprites.add(s);
	check_max();
}

void SpriteSet::del_visible(Sprite *s)
{
	// Remove the sprite from the visible list
	int i = _vis_sprites.find_exact(s);
	ASSERT(i>=0, "SpriteSet::del_visible(): Sprite not found, this shouldn't happen")
	_vis_sprites.remove_nodel(i);
	check_max();
}

void SpriteSet::set_sprite_vertex(struct SpriteVertex *v, Sprite *s)
{
	// Fill in the vertex data
	v->col = s->_col;
	v->u = s->_u1;
	v->v = s->_v1;
	v++;

	v->col = s->_col;
	v->u = s->_u2;
	v->v = s->_v1;
	v++;

	v->col = s->_col;
	v->u = s->_u2;
	v->v = s->_v2;
	v++;

	v->col = s->_col;
	v->u = s->_u1;
	v->v = s->_v2;
	v++;
			
	s->_vtx_dirty = false;
}
	
void SpriteSet::check_max()
{
	// Check if the position vertex buffer should be enlarged
	if(_vis_sprites.size()>_max_size)
	{
		// There are more sprites than the previous max
		// Enlarge and fill the positions static buffer
		int ns = _vis_sprites.size()+256;

		set_pos_data(_max_size, ns-_max_size);

		_max_size = ns;
	}
}
	
void SpriteSet::set_pos_data(int first, int num)
{
	// Fill in position data
	PosVertex *v = (PosVertex*)_va.lock(0, first*4, num*4);

	for(int c = 0; c<num; c++)
	{
		v->x = 0;
		v->y = 0;
		v++;

		v->x = 1;
		v->y = 0;
		v++;

		v->x = 1;
		v->y = 1;
		v++;

		v->x = 0;
		v->y = 1;
		v++;
	}
}

void SpriteSet::draw()
{
	// Set the 2D camera
	Display::get_2d_camera().set();

	// Bind the sprite shader
	_shad->bind();

	// Advance the animations and adjust the sprite data as required
	for(int c = 0; c<_vis_sprites.size(); c++)
	{
		Sprite *s = _vis_sprites[c];

		if(!s->_anim)
			continue;

		AnimFrame *frm = s->_anim->get_frame();

		// Adjust data
		s->_tex = frm->tex;

		if(s->_w!=frm->w || s->_h!=frm->h)
		{
			s->_w = frm->w;
			s->_h = frm->h;
			s->_mat_dirty = true;
		}

		if(s->_u1!=frm->u1 || s->_u2!=frm->u2 || s->_v1!=frm->v1 || s->_v2!=frm->v2)
		{
			s->_u1 = frm->u1;
			s->_v1 = frm->v1;
			s->_u2 = frm->u2;
			s->_v2 = frm->v2;
			s->_vtx_dirty = true;
		}
	}

	// Rebuild the vertex buffer if needed
	if(_va_dirty)
	{
		// Need to rebuild the whole buffers
		
		// Position buffer
		set_pos_data(0, _max_size);

		// Color+Texcoord buffer
		SpriteVertex *v = (SpriteVertex*)_va.lock(1, 0, _vis_sprites.size()*4);
		for(int c = 0; c<_vis_sprites.size(); c++)
		{
			// Set the sprite's vertex data
			set_sprite_vertex(v, _vis_sprites[c]);
			v += 4;
		}

		_va_dirty = false;
	}
	else
	{
		// Check individual sprites that require updating
		for(int c = 0; c<_vis_sprites.size(); c++)
		{
			Sprite *s = _vis_sprites[c];

			if(s->_vtx_dirty)
				set_sprite_vertex((SpriteVertex*)_va.lock(1, c*4, 4), s);
		}
	}

	// Bind the vertex array
	_va.bind(_shad);

	// Draw every sprite
	Texture *last_tex = NULL;

	for(int c = 0; c<_vis_sprites.size(); c++)
	{
		Sprite *s = _vis_sprites[c];

		// Recalculate its matrix ?
		if(s->_mat_dirty)
			s->calc_matrix();

		// Bind the texture if it's different than the last sprite
		if(s->_tex!=last_tex)
		{
			s->_tex->bind(0);
			last_tex = s->_tex;
		}

		// Set the matrix
		_shad->set_uniform("mat", s->_mat);

		// Draw
		Display::draw(c*4, 4);
	}

	VertexArray::unbind();
	Shader::unbind();
}
	
Sprite *SpriteSet::new_sprite(Texture *tex, int z, int x, int y, int ox, int oy, bool vis, float u1, float v1, float u2, float v2)
{
	// Create a new sprite
	Sprite *s = new Sprite(this);

	// Set it up
	s->_tex = tex;
	s->_z = z;
	s->_x = x;
	s->_y = y;
	s->_w = tex->width();
	s->_h = tex->height();
	s->_ox = ox;
	s->_oy = oy;
	s->_visible = vis;
	s->_u1 = u1;
	s->_v1 = v1;
	s->_u2 = u2;
	s->_v2 = v2;

	// Add the sprite to the proper list
	if(vis)
		add_visible(s);
	else
		add_hidden(s);

	return s;
}
	
Sprite *SpriteSet::new_sprite(Anim *anim, int z, int x, int y, int ox, int oy, bool vis)
{
	// Create a new sprite
	AnimFrame *frm = anim->get_frame(false);

	Sprite *s = new_sprite(frm->tex, z, x, y, ox, oy, vis, frm->u1, frm->v1, frm->u2, frm->v2);
	s->_w = frm->w;
	s->_h = frm->h;
	s->_anim = anim;

	return s;
}
	
void SpriteSet::del_sprite(Sprite *s)
{
	// Remove the sprite from whichever list it is
	if(s->_visible)
		del_visible(s);
	else
		del_hidden(s);
}


}


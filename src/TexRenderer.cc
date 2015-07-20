#include "pix5.h"
#include "data.h"

namespace PIX {


typedef struct TRTex
{
	Texture *tex;
	// Texture to render

	int x, y;
	// Destination coordinates

	int w, h;
	// Destination size

	Color col;
	// Tint color

	float u1, v1, u2, v2;
	// Texture coordinates
} TRTex;

typedef struct TRVertex
{
	float x, y;
	float u, v;
	uint32_t col;
} TRVertex;


static Shader *_shad = NULL;
// The shader, created on demand


void tr_init()
{
	// Load the shader
	_shad = Shader::create_from_source("TexRenderer", (char*)data_sprite_vs, data_sprite_vs_len, (char*)data_sprite_fs, data_sprite_fs_len);
}

void tr_done()
{
	// Destroy the shader
	if(_shad)
	{
		delete _shad;
		_shad = NULL;
	}
}


TexRenderer::TexRenderer()
{
	// Setup the vertex array
	VertexDef vd;
		
	vd.add(VertexComp::float2("pos", 0));
	vd.add(VertexComp::float2("texcoord", 8));
	vd.add(VertexComp::ubyte4("col", true, 16));

	_va.add_stream(vd, sizeof(TRVertex), VertexBufferUsage::Stream);
}

TexRenderer::~TexRenderer()
{
	clear();
}

void TexRenderer::clear()
{
	// Clear the queue
	_tex.clear_del();
}

void TexRenderer::add(Texture *tex, int x, int y, const Color& col, int w, int h, float u1, float v1, float u2, float v2)
{
	// Add a new entry
	TRTex *t = new TRTex();

	t->tex = tex;
	t->x = x;
	t->y = y;
	t->w = (w==-1) ? tex->width() : w;
	t->h = (h==-1) ? tex->height() : h;
	t->col = col;
	t->u1 = u1;
	t->v1 = v1;
	t->u2 = u2;
	t->v2 = v2;

	_tex.add(t);

	//printf("@(%i,%i)  Col: 0x%08X   (%ix%i)    [%f,%f] - [%f,%f]\n", x, y, (uint32_t)col, w, h, u1, v1, u2, v2);
}
	
void TexRenderer::add(Anim *anim, AnimState& state, int x, int y, const Color& col, int w, int h, bool advance)
{
	// Get the current frame
	AnimFrame *frm = anim->get_frame(state, advance);

	// Add it
	if(w==-1)
		w = frm->w;
	if(h==-1)
		h = frm->h;

	add(frm->tex, x, y, col, w, h, frm->u1, frm->v1, frm->u2, frm->v2);
}

void TexRenderer::draw(int ox, int oy)
{
	// Fill in the VB data
	TRVertex *v = (TRVertex*)_va.lock(0, 0, 4*_tex.size());
	for(int c = 0; c<_tex.size(); c++)
	{
		TRTex *t = _tex[c];

		// Build the vertices
		v->x = ox+t->x;
		v->y = oy+t->y;
		v->u = t->u1;
		v->v = t->v1;
		v->col = t->col;
		v++;

		v->x = ox+t->x+t->w;
		v->y = oy+t->y;
		v->u = t->u2;
		v->v = t->v1;
		v->col = t->col;
		v++;

		v->x = ox+t->x+t->w;
		v->y = oy+t->y+t->h;
		v->u = t->u2;
		v->v = t->v2;
		v->col = t->col;
		v++;

		v->x = ox+t->x;
		v->y = oy+t->y+t->h;
		v->u = t->u1;
		v->v = t->v2;
		v->col = t->col;
		v++;
	}

	// Set the 2D camera
	Display::get_2d_camera().set();

	// Bind the shader and vertex array
	_shad->bind();
	_va.bind(_shad);

	// Setup the default matrix
	_shad->set_uniform("mat", Matrix::identity());

	// Render all
	Texture *last = NULL;
	for(int c = 0; c<_tex.size(); c++)
	{
		TRTex *t = _tex[c];

		// Bind the texture
		if(t->tex!=last)
		{
			t->tex->bind(0);
			last = t->tex;
		}

		// Draw
		Display::draw(c*4, 4);
	}

	// Unbind everything
	VertexArray::unbind();
	Shader::unbind();
	Texture::unbind(0);

	// Clear the list
	clear();
}



}

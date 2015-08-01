#include "pix5gfx.h"
#include <GL/glew.h>



struct Vertex
{
	float x, y;
	float u, v;
	uint32_t col;
};



VBORenderer::VBORenderer()
{
	_vbo[0] = 0;
	_vbo[1] = 0;
	_size[0] = 0;
	_size[1] = 0;
	_cur_vbo = 0;
}

VBORenderer::~VBORenderer()
{
}

void VBORenderer::on_init()
{
	// Make sure the GPU supports VBOs
	if(!glGenBuffers || !glMapBuffer)
		E::Renderer("VBOs are not supported by the OpenGL context");

	// Create both VBOs
	glGenBuffers(2, _vbo);

	_size[0] = 0;
	_size[1] = 0;
}

void VBORenderer::on_done()
{
	glDeleteBuffers(2, _vbo);
	_vbo[0] = 0;
	_vbo[1] = 0;
}

void VBORenderer::on_prepare()
{
	// Swap which buffer we use
	_cur_vbo = 1-_cur_vbo;

	// Bind it
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[_cur_vbo]);

	// Setup the pointers
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (char*)NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), ((char*)NULL)+8);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), ((char*)NULL)+16);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Should the buffer be enlarged ?
	if(_elements.size()>_size[_cur_vbo])
	{
		// Yes
		_size[_cur_vbo] = _elements.size();

		// Create/resize the buffer
		glBufferData(GL_ARRAY_BUFFER, _size[_cur_vbo]*4*sizeof(Vertex), NULL, GL_STREAM_DRAW);
	}

	// Map the buffer
	Vertex *v = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(!v)
		E::OpenGL("glMapBuffer() failed");

	// Fill in the vertex data
	for(int c = 0; c<_elements.size(); c++)
	{
		RenderElement *el = _elements[c];

		// Top-left
		v->x = el->dx;
		v->y = el->dy;
		v->u = el->u1;
		v->v = el->v1;
		v->col = el->col[0];
		v++;

		// Top-right
		v->x = el->dx+el->w;
		v->y = el->dy;
		v->u = el->u2;
		v->v = el->v1;
		v->col = el->col[1];
		v++;

		// Bottom-right
		v->x = el->dx+el->w;
		v->y = el->dy+el->h;
		v->u = el->u2;
		v->v = el->v2;
		v->col = el->col[2];
		v++;

		// Bottom-left
		v->x = el->dx;
		v->y = el->dy+el->h;
		v->u = el->u1;
		v->v = el->v2;
		v->col = el->col[3];
		v++;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	// Enable textures
	glEnable(GL_TEXTURE_2D);
}
	
void VBORenderer::on_finish()
{
	// Deactivate everything
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBORenderer::on_render(int start, int num)
{
	glDrawArrays(GL_QUADS, start*4, num*4);
}



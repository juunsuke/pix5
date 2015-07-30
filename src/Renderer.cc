#include "pix5.h"
#include <GL/glew.h>

namespace PIX {



Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	clear();
}

void Renderer::init()
{
	on_init();
}

void Renderer::done()
{
	clear();
	on_done();
}

void Renderer::clear()
{
	// Clear the elements list
	_elements.clear_del();
}

void Renderer::render()
{
	// Perform preparations
	on_prepare();

	// Try to batch draws as much as possible
	bool has_identity = false;
	Texture *tex = NULL;

	for(int start = 0; start<_elements.size();)
	{
		RenderElement *el = _elements[start];

		// Assume a single render for now
		int num = 1;

		// Batch draws are only possible for identity elements
		if(el->identity)
		{
			// Count how many elements use the same texture and identity
			for(int c = start+1; c<_elements.size(); c++)
			{
				RenderElement *el2 = _elements[c];

				if(el2->tex==el->tex && el2->identity)
					num++;
				else
					break;
			}
		}

		// Setup the matrix
		if(el->identity)
		{
			// Identity
			if(!has_identity)
			{
				// Set it
				glLoadIdentity();
				has_identity = true;
			}
		}
		else
		{
			// Set the specified matrix
			glLoadMatrixf(el->mat);
			has_identity = false;
		}

		// Bind the texture
		if(el->tex!=tex)
		{
			tex = el->tex;
			tex->bind(0);
		}

		// Do the actual rendering
		on_render(start, num);

		// Keep going
		start += num;
	}

	// Clear the list
	clear();

	on_finish();
}


void Renderer::draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4)
{
	// Create and add a new element with identity matrix
	RenderElement *el = new RenderElement(tex, dx, dy, src_rect, col1, col2, col3, col4);
	el->identity = true;

	_elements.add(el);
}

void Renderer::draw(Texture *tex, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat)
{
	// Create and add a new element with the provided matrix
	RenderElement *el = new RenderElement(tex, 0, 0, src_rect, col1, col2, col3, col4);
	memcpy(el->mat, mat, 16*sizeof(float));

	_elements.add(el);
}

void Renderer::draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale)
{
	// Detect identity for slight performance boost
	if(ox==0 && oy==0 && angle==0 && hscale==1.0f && vscale==1.0f)
	{
		draw(tex, dx, dy, src_rect, col1, col2, col3, col4);
		return;
	}

	// Create and add a new element, building the matrix directly into it
	RenderElement *el = new RenderElement(tex, 0, 0, src_rect, col1, col2, col3, col4);
	build_matrix(el->mat, dx, dy, ox, oy, angle, hscale, vscale);

	_elements.add(el);
}
	
void Renderer::build_matrix(float *mat, int dx, int dy, int ox, int oy, float angle, float hscale, float vscale)
{
	// Build the matrix
	glLoadIdentity();

	// Position
	glTranslatef(dx, dy, 0);

	// Rotation
	if(angle)
		glRotatef(angle, 0, 0, 1.0f);

	// Scaling
	if(hscale!=1.0f || vscale!=1.0f)
		glScalef(hscale, vscale, 1.0f);

	// Origin
	if(ox || oy)
		glTranslatef(-ox, -oy, 0);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
}



}

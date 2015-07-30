#include "pix5.h"
#include <GL/glew.h>


namespace PIX {


ImmRenderer::ImmRenderer()
{
}

ImmRenderer::~ImmRenderer()
{
}

void ImmRenderer::on_init()
{
}

void ImmRenderer::on_done()
{
}
	
void ImmRenderer::on_prepare()
{
	glEnable(GL_TEXTURE_2D);
}

void ImmRenderer::on_finish()
{
	glDisable(GL_TEXTURE_2D);
}

void ImmRenderer::on_render(int start, int num)
{
	// Start rendering directly
	glBegin(GL_QUADS);

	for(int c = 0; c<num; c++)
	{
		// Get the element
		RenderElement *el = _elements[start+c];

		// Top-left
		glColor4ubv((uint8_t*)&el->col[0]);
		glTexCoord2f(el->u1, el->v1);
		glVertex2f(el->dx, el->dy);

		// Top-right
		glColor4ubv((uint8_t*)&el->col[1]);
		glTexCoord2f(el->u2, el->v1);
		glVertex2f(el->dx+el->w, el->dy);

		// Bottom-right
		glColor4ubv((uint8_t*)&el->col[2]);
		glTexCoord2f(el->u2, el->v2);
		glVertex2f(el->dx+el->w, el->dy+el->h);

		// Bottom-left
		glColor4ubv((uint8_t*)&el->col[3]);
		glTexCoord2f(el->u1, el->v2);
		glVertex2f(el->dx, el->dy+el->h);
	}

	// Done with this batch
	glEnd();
}


}

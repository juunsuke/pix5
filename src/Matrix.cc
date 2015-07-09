#include "pix5.h"
#include <GL/glew.h>


namespace PIX {



void Matrix::set_projection() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadTransposeMatrixf(val);
}

void Matrix::set_modelview() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadTransposeMatrixf(val);
}
	
Matrix Matrix::get_projection()
{
	Matrix res;
	glGetFloatv(GL_TRANSPOSE_PROJECTION_MATRIX, res.val);
	return res;
}

Matrix Matrix::get_modelview()
{
	Matrix res;
	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, res.val);
	return res;
}


void Matrix::print() const
{
	printf("+------------------------------------------+\n");
	for(int y = 0; y<4; y++)
	{
		printf("| ");

		for(int x = 0; x<4; x++)
			printf("% 4.4f   ", val[y*4+x]);

		printf(" |\n");
	}
	printf("+------------------------------------------+\n");
}


}




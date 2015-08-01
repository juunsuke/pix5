#pragma once


class RenderElement
{
public:

	Texture *tex;
	// Texture to render

	float dx, dy;
	// Destination coordinates

	float w, h;
	// Destination size

	float u1, v1, u2, v2;
	// Texture coordinates

	uint32_t col[4];
	// The 4 colors

	float mat[16];
	// The matrix

	bool identity;
	// Wether the matrix is identity
	// If this is true, 'mat' is undefined


	RenderElement(Texture *tex, int dx, int dy, const Rect& r, const Color& col1, const Color& col2, const Color& col3, const Color& col4)
	{
		this->tex = tex;
		this->dx = (float)dx;
		this->dy = (float)dy;
		this->w = (float)r.w;
		this->h = (float)r.h;
		this->col[0] = col1;
		this->col[1] = col2;
		this->col[2] = col3;
		this->col[3] = col4;
		this->identity = false;

		// Calc U/V coordinates
		float du = 1.0f / (float)tex->width();
		float dv = 1.0f / (float)tex->height();

		u1 = (float)r.x * du;
		v1 = (float)r.y * dv;
		u2 = (float)(r.x+r.w) * du;
		v2 = (float)(r.y+r.h) * dv;
	}

	~RenderElement()
	{
	}
};



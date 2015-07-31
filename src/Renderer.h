#pragma once


class Renderer: public Obj
{
protected:

	List<class RenderElement*> _elements;
	// List of queued elements to be rendered

	Renderer();

	virtual void on_init() = 0;
	virtual void on_done() = 0;
	// Perform renderer-specific init/shutdown

	virtual void on_prepare() {}
	// Perform pre-render preparations

	virtual void on_finish() {}
	// Perform any post-render cleanups

	virtual void on_render(int start, int num) = 0;
	// Render the specified range of elements
	// The matrix and texture are already set

public:

	~Renderer();


	void init();
	void done();
	// Init/shutdown the renderer
	// Do NOT call these functions, they are used internally and must be public for access

	void clear();
	// Clear all the queued elements

	void render();
	// Render all the queued elements and clear the list


	static void build_matrix(float *mat, int dx, int dy, int ox, int oy, float angle, float hscale, float vscale);
	// Build a matrix given the transformation coordinates
	// 'mat' must point to an array of 16 float values


	//
	// No transformation besides position
	//

	void draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4);

	inline void draw(Texture *tex, int dx, int dy, const Color& col1, const Color& col2, const Color& col3, const Color& col4)
	{ draw(tex, dx, dy, Rect(0, 0, tex->width(), tex->height()), col1, col2, col3, col4); }

	inline void draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col)
	{ draw(tex, dx, dy, src_rect, col, col, col, col); }

	inline void draw(Texture *tex, int dx, int dy, const Rect& src_rect)
	{ draw(tex, dx, dy, src_rect, Color(1, 1, 1, 1)); }

	inline void draw(Texture *tex, int dx, int dy, const Color& col)
	{ draw(tex, dx, dy, Rect(0, 0, tex->width(), tex->height()), col); }

	inline void draw(Texture *tex, int dx, int dy)
	{ draw(tex, dx, dy, Color(1, 1, 1, 1)); }



	//
	// Provided transformation matrix
	//

	void draw(Texture *tex, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat);

	inline void draw(Texture *tex, const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat)
	{ draw(tex, Rect(0, 0, tex->width(), tex->height()), col1, col2, col3, col4, mat); }

	inline void draw(Texture *tex, const Rect& src_rect, const Color& col, float *mat)
	{ draw(tex, src_rect, col, col, col, col, mat); }

	inline void draw(Texture *tex, const Rect& src_rect, float *mat)
	{ draw(tex, src_rect, Color(1, 1, 1, 1), mat); }

	inline void draw(Texture *tex, const Color& col, float *mat)
	{ draw(tex, Rect(0, 0, tex->width(), tex->height()), col, mat); }

	inline void draw(Texture *tex, float *mat)
	{ draw(tex, Color(1, 1, 1, 1), mat); }


	
	//
	// Provided raw transformations
	//

	void draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale);

	inline void draw(Texture *tex, int dx, int dy, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(tex, dx, dy, Rect(0, 0, tex->width(), tex->height()), col1, col2, col3, col4, ox, oy, angle, hscale, vscale); }

	inline void draw(Texture *tex, int dx, int dy, const Rect& src_rect, const Color& col, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(tex, dx, dy, src_rect, col, col, col, col, ox, oy, angle, hscale, vscale); }

	inline void draw(Texture *tex, int dx, int dy, const Rect& src_rect, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(tex, dx, dy, src_rect, Color(1, 1, 1, 1), ox, oy, angle, hscale, vscale); }

	inline void draw(Texture *tex, int dx, int dy, const Color& col, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(tex, dx, dy, Rect(0, 0, tex->width(), tex->height()), col, ox, oy, angle, hscale, vscale); }

	inline void draw(Texture *tex, int dx, int dy, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(tex, dx, dy, Color(1, 1, 1, 1), ox, oy, angle, hscale, vscale); }



};


#pragma once


// Texture filter type, used for min and mag filters
// Used when the texture is scaled or rotated, so that the GPU can know
// how to render each pixel
namespace TextureFilter
{
	enum Type
	{
		Nearest,
		// Uses the nearest pixel, faster but looks blockier
		// This is the default

		Linear
		// Interpolates surrounding pixels, slower but looks better
	};
}

// Texture wrap type
// Used when texture coordinates greater than 1 are used
namespace TextureWrap
{
	enum Type
	{
		Clamp,
		// Beyond 1, no texture data will be used

		Repeat,
		// Beyond 1, the texture data will be repeated (integer part of the
		// texture coordinate is ignored)
		// This is the default

		Mirror
		// Like repeat, but for odd integer parts, the teture is reversed
	};
}


class Texture
{
	int _w, _h;
	// Texture size

	Str _file;
	// File name, when the texture was loaded

	uint32_t *_data;
	// Raw data, in ABGR format

	uint32_t _gl;
	// OpenGL texture ID

	bool _dirty;
	// Wether the texture is considered dirty and should be re-uploaded to OpenGL

	TextureFilter::Type _min_filter, _mag_filter;
	// Min/mag filters

	TextureWrap::Type _wrap_h, _wrap_v;
	// Horizontal/vertical wrapping

	Rect _clip;
	// Clipping rectangle, for drawing operations


	
	Texture();
	// Empty texture, used internally

	void create_gl();
	// Create/bind/configure/upload the OpenGL texture

	void delete_gl();
	// Delete the OpenGL texture, if it exists

	void hline(int x1, int x2, int y, const Color& col);
	void vline(int y1, int y2, int x, const Color& col);

	void blit_raw(uint32_t *dptr, uint32_t *sptr, int w, int h, int spitch);
	void blit_alpha(uint32_t *dptr, uint32_t *sptr, int w, int h, int spitch);

public:

	static Texture *create(int width, int height, bool clear = true);
	// Create a new texture with the given size
	// If 'clear' is true, the texture will be cleared black (with 0 alpha)
	// The minimum size for a texture is 1x1
	// Textures can be created prior to setting a mode, as such, this function
	// cannot verify the maximum texture size supported by the GPU, and will
	// not impose a maximum size
	// Similarily, the function cannot verify if the GPU supports Non-Power-Of-2
	// textures, and will allow them
	// Those check will be performed when the texture is uploaded to OpenGL,
	// mainly in Texture::bind()

	static Texture *load(const Str& fname);
	// Load a texture from an imagie file
	// Officially, PIX5 only supports .PNG image files, however, since for the
	// moment SDL2_image is used, any format it supports will be accepted
	// The same remarks as Texture::create() regarding texture sizes also apply
	// to images loaded by this function

	static Texture *cache(const Str& name);
	// Get a texture from the cache
	// If it doesn't exist, load it from the file 'name'


	~Texture();


	void add_cache(const Str& name);
	// Add this texture to the cache with the given name


	Texture *sub(const Rect& r);
	inline Texture *sub(int x, int y, int w, int h) { return sub(Rect(x, y, w, h)); }
	// Create a sub-texture
	

	inline Str file() const { return _file; }
	// Get the filed name, when the texture was loaded

	inline int width() const { return _w; }
	inline int height() const { return _h; }
	// Return the texture size

	inline bool is_dirty() const { return _dirty; }
	// Return the dirty flag

	inline TextureFilter::Type get_min_filter() const { return _min_filter; }
	inline TextureFilter::Type get_mag_filter() const { return _mag_filter; }
	// Get the min/mag filter

	inline TextureWrap::Type get_h_wrap() const { return _wrap_h; }
	inline TextureWrap::Type get_v_wrap() const { return _wrap_v; }
	// Get the horizontal/vertical wrap
	
	void set_filter(TextureFilter::Type min, TextureFilter::Type mag);
	// Set the min/mag filters
	// This may cause the OpenGL texture to be destroyed and later re-created

	void set_wrap(TextureWrap::Type horiz, TextureWrap::Type vert);
	// Set the horizontal/vertical wrap
	// This may cause the OpenGL texture to be destroyed and later re-created

	inline Rect get_clip() const { return _clip; }
	// Get the clipping rectangle

	void set_clip(const Rect& r);
	// Set the clipping rectangle
	// It is adjusted to make sure it fits inside the texture

	void bind(int index);
	// Bind the texture in OpenGL using GL_TEXTURE_2D
	// 'index' represents the sampler unit to use
	// A video mode must be set for this function to work
	// If the OpenGL texture was not already created, it will be created
	// If the texture data was changed, it will be re-uploaded to OpenGL
	// When creating the texture, this function validates the texture dimensions
	// Some GPU may force textures to only be power-of-two in width and height,
	// and all have a different maximum size
	// An error will be thrown if OpenGL refuses the texture based on its size
	// If there was no bound texture, GL_TEXTURE_2D will be enabled within OpenGL
	// This will leave the active texture unit 'index' selected

	static void unbind(int index);
	// Unbind any currently bound texture in OpenGL using GL_TEXTURE_2D
	// If there was a bound texture, GL_TEXTURE_2D will be disabled within OpenGL
	// This will leave the active texture unit 'index' selected

	void clear(const Color& col);
	// Clear the texture using the given color

	void set_dirty() { _dirty = true; }
	// Set the dirty flag

	inline void set_pixel_fast(int x, int y, uint32_t col)
	{
		// This sets a single pixel as fast as possible
		// No clipping is performed
		// The dirty flag is not updated
		// Use with care
		_data[y*_w+x] = col;
	}

	inline uint32_t get_pixel_fast(int x, int y)
	{
		// This gets the color of a single pixel as fast as possible
		// No clipping is performed
		// Used with care
		return _data[y*_w+x];
	}


	//
	// Drawing functions
	// All these functions modify the raw texture data, and marks it as dirty
	// The modified texture will be re-uploaded to OpenGL on the next bind()
	// All these functions take account of the clipping rectangle
	//

	void set_pixel(int x, int y, const Color& col);
	// Set a single pixel

	Color get_pixel(int x, int y);
	// Get a single pixel
	// If out of bounds, returns 0

	void line(int x1, int y1, int x2, int y2, const Color& col);
	// Draw a regular line

	void line(int x1, int y1, int x2, int y2, float width, const Color& col);
	// Draw an anti-aliased line of arbitrary thickness

	void rect(int x1, int y1, int x2, int y2, const Color& col);
	inline void rect(const Rect& r, const Color& col) { rect(r.x, r.y, r.x+r.w-1, r.y+r.h-1, col); }
	// Draw a rectangle outline

	void rect_fill(int x1, int y1, int x2, int y2, const Color& col);
	inline void rect_fill(const Rect& r, const Color& col) { rect_fill(r.x, r.y, r.x+r.w-1, r.y+r.h-1, col); }
	// Draw a filled rectangle

	void circle(int x, int y, int r, const Color& col);
	// Draw a circle outline, with (x,y) as a center and 'r' as a radius

	void circle_fill(int x, int y, int r, const Color& col);
	// Draw a filled circle, with (x,y) as a center and 'r' as a radius

	inline void print(Font *fnt, int x, int y, const Color& col, const Str& text, bool kerning = true) { fnt->print(this, x, y, col, text, kerning); }
	// Print a string using the given font

	void blit(int dx, int dy, Texture *src, int sx, int sy, int w, int h, bool alpha = true);
	inline void blit(int dx, int dy, Texture *src, const Rect& sr, bool alpha = true) { blit(dx, dy, src, sr.x, sr.y, sr.w, sr.h, alpha); }
	inline void blit(int dx, int dy, Texture *src, bool alpha = true) { blit(dx, dy, src, 0, 0, src->_w, src->_h, alpha); }


	//
	// Rendering functions
	// These are just simple forwards for the display renderer
	//

	//
	// No transformation besides position
	//

	void draw(int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4);

	inline void draw(int dx, int dy, const Color& col1, const Color& col2, const Color& col3, const Color& col4)
	{ draw(dx, dy, Rect(0, 0, _w, _h), col1, col2, col3, col4); }

	inline void draw(int dx, int dy, const Rect& src_rect, const Color& col)
	{ draw(dx, dy, src_rect, col, col, col, col); }

	inline void draw(int dx, int dy, const Rect& src_rect)
	{ draw(dx, dy, src_rect, Color(1, 1, 1, 1)); }

	inline void draw(int dx, int dy, const Color& col)
	{ draw(dx, dy, Rect(0, 0, _w, _h), col); }

	inline void draw(int dx, int dy)
	{ draw(dx, dy, Color(1, 1, 1, 1)); }



	//
	// Provided transformation matrix
	//

	void draw(const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat);

	inline void draw(const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat)
	{ draw(Rect(0, 0, _w, _h), col1, col2, col3, col4, mat); }

	inline void draw(const Rect& src_rect, const Color& col, float *mat)
	{ draw(src_rect, col, col, col, col, mat); }

	inline void draw(const Rect& src_rect, float *mat)
	{ draw(src_rect, Color(1, 1, 1, 1), mat); }

	inline void draw(const Color& col, float *mat)
	{ draw(Rect(0, 0, _w, _h), col, mat); }

	inline void draw(float *mat)
	{ draw(Color(1, 1, 1, 1), mat); }


	
	//
	// Provided raw transformations
	//

	void draw(int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale);

	inline void draw(int dx, int dy, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(dx, dy, Rect(0, 0, _w, _h), col1, col2, col3, col4, ox, oy, angle, hscale, vscale); }

	inline void draw(int dx, int dy, const Rect& src_rect, const Color& col, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(dx, dy, src_rect, col, col, col, col, ox, oy, angle, hscale, vscale); }

	inline void draw(int dx, int dy, const Rect& src_rect, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(dx, dy, src_rect, Color(1, 1, 1, 1), ox, oy, angle, hscale, vscale); }

	inline void draw(int dx, int dy, const Color& col, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(dx, dy, Rect(0, 0, _w, _h), col, ox, oy, angle, hscale, vscale); }

	inline void draw(int dx, int dy, int ox, int oy, float angle, float hscale, float vscale)
	{ draw(dx, dy, Color(1, 1, 1, 1), ox, oy, angle, hscale, vscale); }
};




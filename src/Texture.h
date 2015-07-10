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


	
	Texture();
	// Empty texture, used internally

	Texture(const Texture& o) { }
	Texture& operator=(const Texture& o) { return *this; }
	// No copy constructor or affectation available

	void create_gl();
	// Create/bind/configure/upload the OpenGL texture

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

	~Texture();
	
	void delete_gl();
	// Delete the OpenGL texture, if it exists


	inline int width() const { return _w; }
	inline int height() const { return _h; }
	// Return the texture size

	inline bool is_dirty() const { return _dirty; }
	// Return the dirty flag

	inline uint32_t get_gl_id() const { return _gl; }
	// Return the OpenGL texture ID (GL_TEXTURE_2D)
	// bind() should be called prior to this to make sure it has been created

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

};




#include "pix5.h"
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>


namespace PIX {



Texture::Texture()
{
	// Empty texture
	_w = 0;
	_h = 0;
	_data = NULL;
	_gl = 0;
	_dirty = true;
	_min_filter = TextureFilter::Nearest;
	_mag_filter = TextureFilter::Nearest;
	_wrap_h = TextureWrap::Repeat;
	_wrap_v = TextureWrap::Repeat;
}

Texture::~Texture()
{
	// Free a texture
	if(_data)
		free(_data);

	delete_gl();
}

Texture *Texture::create(int width, int height, bool clear)
{
	// Create a new texture
	if(width<1 || height<1)
		E::TextureSize("Texture::create(): width and height must be at least 1");

	Texture *tex = new Texture();
	tex->_w = width;
	tex->_h = height;
	tex->_data = (uint32_t*)malloc(width*height*4);
	tex->_dirty = true;

	// Clear ?
	if(clear)
		memset(tex->_data, 0, width*height*4);

	return tex;
}
	
Texture *Texture::load(const Str& fname)
{
	// Try to load the given image file
	SDL_Surface *sur = IMG_Load(fname);
	if(!sur)
		E::LoadTexture("%s", IMG_GetError());

	// Validate the size just to be sure
	if(sur->w<1 || sur->h<1)
		E::LoadTexture("The loaded image has invalid dimensions: '%s'", fname.ptr());

	// Convert the surface to the proper format
	SDL_Surface *sur2 = SDL_ConvertSurfaceFormat(sur, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(sur);
	sur = sur2;

	if(!sur)
		E::LoadTexture("Error converting the surface image format for '%s': %s", fname.ptr(), SDL_GetError());

	// Create a new, empty texture
	Texture *tex = Texture::create(sur->w, sur->h, false);

	// Copy the surface data
	SDL_LockSurface(sur);

	uint32_t *dptr = tex->_data;
	uint8_t *sptr = (uint8_t*)sur->pixels;

	for(int y = 0; y<sur->h; y++)
	{
		memcpy(dptr, sptr, sur->w*4);

		sptr += sur->pitch;
		dptr += sur->w;
	}

	// All done
	SDL_UnlockSurface(sur);
	SDL_FreeSurface(sur);

	Log::log("Loaded '%s': %ix%i", fname.ptr(), tex->_w, tex->_h);

	return tex;
}
	
void Texture::delete_gl()
{
	// Delete the OpenGL texture if it exists
	if(_gl)
	{
		glDeleteTextures(1, &_gl);
		_gl = 0;
	}
}

void Texture::set_filter(TextureFilter::Type min, TextureFilter::Type mag)
{
	// Don't do anything if they didn't change
	if(min==_min_filter && mag==_mag_filter)
		return;

	// Change them
	_min_filter = min;
	_mag_filter = mag;

	// Drop the OpenGL texture if it exists
	delete_gl();
}
	
void Texture::set_wrap(TextureWrap::Type horiz, TextureWrap::Type vert)
{
	// Don't do anything if they didn't change
	if(_wrap_h==horiz && _wrap_v==vert)
		return;

	// Change them
	_wrap_h = horiz;
	_wrap_v = vert;

	// Drop the OpenGL texture if it exists
	delete_gl();
}

inline static GLint gl_filter(TextureFilter::Type filter)
{
	// Get the OpenGL filter value
	switch(filter)
	{
		case TextureFilter::Linear:			return GL_LINEAR;
		default:							return GL_NEAREST;
	}
}

inline static GLint gl_wrap(TextureWrap::Type wrap)
{
	// Get the OpenGL wrap value
	switch(wrap)
	{
		case TextureWrap::Clamp:			return GL_CLAMP_TO_EDGE;
		case TextureWrap::Mirror:			return GL_MIRRORED_REPEAT;
		default:							return GL_REPEAT;
	}
}

static bool is_pot(int v)
{
	// Check if the value is power of two
	while(!(v&1))
		v>>=1;

	if(v-1)
		return false;

	return true;
}

void Texture::create_gl()
{
	// Create an OpenGL texture
	glGenTextures(1, &_gl);

	// Bind it
	glBindTexture(GL_TEXTURE_2D, _gl);

	// Set the parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter(_min_filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter(_mag_filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap(_wrap_h));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap(_wrap_v));

	// Upload the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);

	// Check for errors
	if(glGetError()!=GL_NO_ERROR)
	{
		// Some error occured
		// Check the max texture size supported by the GPU
		int sz = Display::get_max_texture_size();
		if(_w>sz)
			E::TextureSize("Texture::bind(): The texture's width is larger than the maximum supported texture size of the GPU (width:%i max:%i)", _w, sz);
		if(_h>sz)
			E::TextureSize("Texture::bind(): The texture's height is larger than the maximum supported texture size of the GPU (height:%i max:%i)", _h, sz);

		// Check if the GPU doesn't support NPOT textures
		if(!GLEW_ARB_texture_non_power_of_two)
		{
			// It doesn't, check if either dimension isn't power of 2
			if(!is_pot(_w))
				E::TextureSize("Texture::bind(): The GPU does not support non-power-of-two (NPOT) textures, and the texture width is %i", _w);
			if(!is_pot(_h))
				E::TextureSize("Texture::bind(): The GPU does not support non-power-of-two (NPOT) textures, and the texture height is %i", _h);
		}

		E::OpenGL("Texture::bind(): Error uploading texture to OpenGL");
	}

	_dirty = false;
}

void Texture::bind(int index)
{
	// Make sure there is a mode
	ASSERT(Display::get_window(), "Texture::bind(): No mode currently set")

	glActiveTexture(GL_TEXTURE0+index);

	// Create the texture if it doesn't exist
	if(!_gl)
	{
		create_gl();
		return;
	}

	// Bind
	glBindTexture(GL_TEXTURE_2D, _gl);

	// Re-upload if its marked as dirty
	if(_dirty)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, _data);
		_dirty = false;
	}
}
	
void Texture::unbind(int index)
{
	ASSERT(Display::get_window(), "Texture::unbind(): No mode currently set")

	// Unbind
	glActiveTexture(GL_TEXTURE0+index);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable texturing
	glDisable(GL_TEXTURE_2D);
}

void Texture::clear(const Color& col)
{
	// Clear the texture
	uint32_t v = col;
	uint32_t *ptr = _data;

	// Fill the first row
	for(int x = 0; x<_w; x++)
		*(ptr++) = v;

	// Copy over the other rows
	for(int y = 1; y<_h; y++)
	{
		memcpy(ptr, _data, _w*4);
		ptr += _w;
	}

	_dirty = true;
}



}

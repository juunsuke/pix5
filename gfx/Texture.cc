#include "pix5gfx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>




Texture::Texture()
{
	// Empty texture
	_w = 0;
	_h = 0;
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
	tex->_data.resize(width*height, clear);
	tex->_dirty = true;
	tex->_clip = Rect(0, 0, width, height);

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
	tex->_file = fname;

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

static void kill_tex(const Str& name, void *ptr)
{
	delete (Texture*)ptr;
}
	
Texture *Texture::cache(const Str& name)
{
	// Check if it already is in the cache
	Texture *tex = (Texture*)Cache::get(name);

	if(tex)
		return tex;

	// No, try to load it
	tex = Texture::load(name);

	// Cache it
	Cache::add(name, tex, kill_tex);

	return tex;
}
	
void Texture::add_cache(const Str& name)
{
	// Add this texture to the cache
	Cache::add(name, this, kill_tex);
}
	
Texture *Texture::sub(const Rect& r)
{
	ASSERT(r.w>=1 && r.h>=1, "Texture::sub(): Invalid dimensions")
	ASSERT(r.x>=0 && (r.x+r.w)<=_w && r.y>=0 && (r.y+r.h)<=_h, "Texture::sub(): The given region lies outside of the texture")

	// Create a texture and blit a portion of us on it
	Texture *tex = Texture::create(r.w, r.h, false);
	tex->blit(0, 0, this, r.x, r.y, r.w, r.h, false);

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
	
void Texture::release_video_data()
{
	delete_gl();
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
	
void Texture::set_clip(const Rect& r)
{
	// Clip the clipping rectangle
	if(!Rect::clip(r, Rect(0, 0, _w, _h), _clip))
		_clip = Rect(0, 0, 0, 0);
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
		memcpy(ptr, _data.ptr(), _w*4);
		ptr += _w;
	}

	_dirty = true;
}
	
void Texture::set_pixel(int x, int y, const Color& col)
{
	if(_clip.contains(x, y))
	{
		set_pixel_fast(x, y, col);
		_dirty = true;
	}
}

Color Texture::get_pixel(int x, int y)
{
	if(_clip.contains(x, y))
		return get_pixel_fast(x, y);

	return 0;
}

void Texture::hline(int x1, int x2, int y, const Color& col)
{
	if(x1>x2)
		Math::swap(x1, x2);

	// Clip
	if(y<_clip.y || y>=(_clip.y+_clip.h))
		return;
	if(x1>=(_clip.x+_clip.w))
		return;
	if(x2<_clip.x)
		return;

	if(x1<_clip.x)
		x1 = _clip.x;
	if(x2>=(_clip.x+_clip.w))
		x2 = _clip.x+_clip.w-1;

	uint32_t *ptr = _data.ptr() + y*_w + x1;
	for(int w = x2-x1+1; w>0; w--)
		*(ptr++) = col;

	_dirty = true;
}

void Texture::vline(int y1, int y2, int x, const Color& col)
{
	if(y1>y2)
		Math::swap(y1, y2);

	// Clip
	if(x<_clip.x || x>=(_clip.x+_clip.w))
		return;
	if(y1>=(_clip.y+_clip.h))
		return;
	if(y2<_clip.y)
		return;

	if(y1<_clip.y)
		y1 = _clip.y;
	if(y2>=(_clip.y+_clip.h))
		y2 = _clip.y+_clip.h-1;

	uint32_t *ptr = _data.ptr() + y1*_w + x;
	for(int h = y2-y1+1; h>0; h--)
	{
		*ptr = col;
		ptr += _w;
	}

	_dirty = true;
}

void Texture::line(int x1, int y1, int x2, int y2, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	if(x1==x2 && y1==y2)
	{
		set_pixel(x1, y1, col);
		return;
	}

	if(x1==x2)
	{
		vline(y1, y2, x1, col);
		return;
	}

	if(y1==y2)
	{
		hline(x1, x2, y1, col);
		return;
	}

	// Use Bresenham to draw a regular line
	// Taken from http://members.chello.at/easyfilter/bresenham.html

	int sx = x1<x2 ? 1 : -1;
	int sy = y1<y2 ? 1 : -1;
	int dx = abs(x2-x1);
	int dy = -abs(y2-y1);
	int err = dx+dy;

	for(;;)
	{
		if(_clip.contains(x1, y1))
			set_pixel_fast(x1, y1, col);

		if(x1==x2 && y1==y2)
			break;

		int e2 = err*2;

		if(e2>=dy)
		{
			err += dy;
			x1 += sx;
		}
		if(e2<=dx)
		{
			err += dx;
			y1 += sy;
		}
	}

	_dirty = true;
}
	
void Texture::line(int x1, int y1, int x2, int y2, float width, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	// Use Bresenham to draw a regular line
	// Taken from http://members.chello.at/easyfilter/bresenham.html

	int sx = x1<x2 ? 1 : -1;
	int sy = y1<y2 ? 1 : -1;
	int dx = abs(x2-x1);
	int dy = abs(y2-y1);
	int err = dx-dy;
	float ed = (dx+dy)==0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
	width = (width+1)/2;

	for(;;)
	{
		if(_clip.contains(x1, y1))
			set_pixel_fast(x1, y1, Color::mix(get_pixel_fast(x1, y1), col, (float)(255-Math::max(0, 255*(abs(err-dx+dy)/ed-width+1)))/255.0f));

		int e2 = err;
		int x3 = x1;

		if(e2*2 >= -dx)
		{
			e2 += dy;
			for(int y3 = y1; e2<ed*width && (y2!=y3 || dx>dy); e2 += dx)
			{
				y3 += sy;

				if(_clip.contains(x1, y3))
					set_pixel_fast(x1, y3, Color::mix(get_pixel_fast(x1, y3), col, (float)(255-Math::max(0, 255*(abs(e2)/ed-width+1)))/255.0f));
			}

			if(x1==x2)
				break;

			e2 = err;
			err -= dy;
			x1 += sx;
		}

		if(e2*2 <= dy)
		{
			for(e2 = dx-e2; e2<ed*width && (x2!=x3 || dx<dy); e2 += dy)
			{
				x3 += sx;

				if(_clip.contains(x3, y1))
					set_pixel_fast(x3, y1, Color::mix(get_pixel_fast(x3, y1), col, (float)(255-Math::max(0, 255*(abs(e2)/ed-width+1)))/255.0f));
			}

			if(y1==y2)
				break;

			err += dx;
			y1 += sy;
		}
	}

	_dirty = true;
}

void Texture::rect(int x1, int y1, int x2, int y2, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	// Single v line?
	if(x1==x2)
	{
		vline(y1, y1, x1, col);
		return;
	}

	// Single h line?
	if(y1==y2)
	{
		hline(x1, x2, y1, col);
		return;
	}

	// Draw 4 lines
	hline(x1, x2, y1, col);
	hline(x1, x2, y2, col);
	vline(y1, y2, x1, col);
	vline(y1, y2, x2, col);
}

void Texture::rect_fill(int x1, int y1, int x2, int y2, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	if(x1>x2)
		Math::swap(x1, x2);
	if(y1>y2)
		Math::swap(y1, y2);
	
	// Clip
	Rect r;
	if(!Rect::clip(Rect(x1, y1, x2-x1+1, y2-y1+1), _clip, r))
		return;

	// Draw using hlines
	for(int y = 0; y<r.h; y++)
		hline(r.x, r.x+r.w-1, r.y+y, col);
}

void Texture::circle(int x, int y, int r, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	Rect dummy;
	if(!Rect::clip(Rect(x-r, y-r, r*2, r*2), _clip, dummy))
		return;

	int hr = (int)((float)r*0.70710678);
	if(Rect(x-hr, y-hr, hr*2, hr*2).contains(_clip))
		return;
	
	// Use Bresenham to draw a circle outline
	// Taken from http://members.chello.at/easyfilter/bresenham.html

	int xm = x;
	int ym = y;

	x = -r;
	y = 0;
	int err = 2-2*r;

	for(;;)
	{
		if(_clip.contains(xm-x, ym+y))	set_pixel_fast(xm-x, ym+y, col);
		if(_clip.contains(xm-y, ym-x))	set_pixel_fast(xm-y, ym-x, col);
		if(_clip.contains(xm+x, ym-y))	set_pixel_fast(xm+x, ym-y, col);
		if(_clip.contains(xm+y, ym+x))	set_pixel_fast(xm+y, ym+x, col);

		r = err;
		if(r<=y)
			err += ++y*2+1;
		if(r>x || err>y)
			err += ++x*2+1;

		if(x>=0)
			break;
	}

	_dirty = true;
}

void Texture::circle_fill(int x, int y, int r, const Color& col)
{
	// Check for full clips right away
	if(_clip.w==0 || _clip.h==0)
		return;

	Rect dummy;
	if(!Rect::clip(Rect(x-r, y-r, r*2, r*2), _clip, dummy))
		return;

	int hr = (int)((float)r*0.70710678);
	if(Rect(x-hr, y-hr, hr*2, hr*2).contains(_clip))
	{
		// The circle full encompasses the clipping rectangle, fill it all
		rect_fill(_clip, col);
		return;
	}
	
	// Use Bresenham to draw a filled circle
	// Taken from http://members.chello.at/easyfilter/bresenham.html

	int xm = x;
	int ym = y;

	x = -r;
	y = 0;
	int err = 2-2*r;

	for(;;)
	{
		// Draw 2 horizontal lines
		hline(xm+x, xm-x, ym-y, col);
		hline(xm-y, xm+y, ym-x, col);

		r = err;
		if(r<=y)
			err += ++y*2+1;
		if(r>x || err>y)
			err += ++x*2+1;

		if(x>=0)
			break;
	}
}

void Texture::blit_raw(uint32_t *dptr, uint32_t *sptr, int w, int h, int spitch)
{
	// Perform a raw blit
	for(int y = 0; y<h; y++)
	{
		memcpy(dptr, sptr, w*4);
		dptr += _w;
		sptr += spitch;
	}
}

void Texture::blit_alpha(uint32_t *dptr, uint32_t *sptr, int w, int h, int spitch)
{
	// Perform an alpha blit
	for(int y = 0; y<h; y++)
	{
		for(int x = 0; x<w; x++)
			dptr[x] = Color::mix(dptr[x], sptr[x]);

		dptr += _w;
		sptr += spitch;
	}
}

void Texture::blit(int dx, int dy, Texture *src, int sx, int sy, int w, int h, bool alpha)
{
	// Clip
	Rect r;
	int ox, oy;

	if(!Rect::clip(Rect(dx, dy, w, h), _clip, r, &ox, &oy))
		return;

	dx = r.x;
	dy = r.y;
	w = r.w;
	h = r.h;

	sx += ox;
	sy += oy;

	if(!Rect::clip(Rect(sx, sy, w, h), src->_clip, r, &ox, &oy))
		return;

	sx = r.x;
	sy = r.y;

	w = r.w;
	h = r.h;

	dx += ox;
	dy += oy;


	// Perform the proper blit
	if(alpha)
		blit_alpha(_data.ptr()+dy*_w+dx, src->_data.ptr()+sy*src->_w+sx, w, h, src->_w);
	else
		blit_raw(_data.ptr()+dy*_w+dx, src->_data.ptr()+sy*src->_w+sx, w, h, src->_w);

	_dirty = true;
}

void Texture::draw(int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4)
{
	Display::renderer()->draw(this, dx, dy, src_rect, col1, col2, col3, col4);
}

void Texture::draw(const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, float *mat)
{
	Display::renderer()->draw(this, src_rect, col1, col2, col3, col4, mat);
}

void Texture::draw(int dx, int dy, const Rect& src_rect, const Color& col1, const Color& col2, const Color& col3, const Color& col4, int ox, int oy, float angle, float hscale, float vscale)
{
	Display::renderer()->draw(this, dx, dy, src_rect, col1, col2, col3, col4, ox, oy, angle, hscale, vscale);
}




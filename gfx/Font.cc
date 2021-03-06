#include "pix5gfx.h"
#include <ft2build.h>
#include FT_FREETYPE_H



static FT_Library _ft = NULL;
// Freetype library handle


void font_init()
{
	// Init freetype
	if(FT_Init_FreeType(&_ft))
		E::FreeType("Error initializing the FreeType library");
}

void font_done()
{
	if(_ft)
	{
		FT_Done_FreeType(_ft);
		_ft = NULL;
	}
}

	
Font::Font()
{
	_face = NULL;
}
	
Font *Font::load(const Str& fname, int ptsize)
{
	// Try to load the face
	Log::log("Loading font: %s", fname.ptr());

	FT_Face face;
	if(FT_New_Face(_ft, fname, 0, &face))
		E::FreeType("Error loading font: %s", fname.ptr());

	Font *fnt = new Font();
	fnt->setup(face, ptsize);

	return fnt;
}
	
Font *Font::load(const void *buf, int size, int ptsize)
{
	// Try to load the face
	Log::log("Loading memory font");

	FT_Face face;
	if(FT_New_Memory_Face(_ft, (const FT_Byte*)buf, size, 0, &face))
		E::FreeType("Error loading a memory font");

	Font *fnt = new Font();
	fnt->setup(face, ptsize);

	return fnt;
}

Font::~Font()
{
	// Free the glyphs
	for(int c = 0; c<_glyphs.size(); c++)
		if(_glyphs[c])
			delete _glyphs[c];

	// Free the face
	if(_face)
		FT_Done_Face(_face);
}

static void kill_font(const Str& name, void *ptr)
{
	delete (Font*)ptr;
}
	
Font *Font::cache(const Str& name, int ptsize)
{
	// Append the point size to the name
	Str cname = Str::build("%s_%i", name.ptr(), ptsize);

	// Check if it already is in the cache
	Font *fnt = (Font*)Cache::get(cname);

	if(fnt)
		return fnt;

	// No, try to load it
	fnt = Font::load(name, ptsize);

	// Cache it
	Cache::add(cname, fnt, kill_font);

	return fnt;
}
	
void Font::setup(struct FT_FaceRec_* face, int ptsize)
{
	// Set the face
	_face = face;

	// Set the size
	if(FT_Set_Char_Size(_face, 0, ptsize*64, 96, 96))
		E::FreeType("Error setting the font size to %i points", ptsize);

	// Allocate the glyphs buffers
	int num = _face->num_glyphs;
	_glyphs.resize(num, true);

	// Get the height and spacing
	_height = (_face->size->metrics.ascender - _face->size->metrics.descender)/64;
	_spacing = _face->size->metrics.height/64;
	
	#ifdef DBG
	Log::debug("Success. %i glyphs, %i sizes, %i fixed sizes", num, _face->available_sizes, _face->num_fixed_sizes);
	#endif

}

Glyph *Font::get_glyph(int ch)
{
	// Get the glyph index for the given character
	int i = FT_Get_Char_Index(_face, ch);

	ASSERT(i>=0 && i<_glyphs.size(), "FT_Get_Char_Index() returned an out-of-range index, this shouldn't happen")

	// Return the glyph if it's already loaded
	if(_glyphs[i])
		return _glyphs[i];

	// Load the glyph
	if(FT_Load_Glyph(_face, i, FT_LOAD_RENDER))
		E::FreeType("Error loading glyph index %i", i);

	// Build the glyph
	Glyph *g = new Glyph();
	g->index = i;

	_glyphs[i] = g;

	FT_Bitmap *bmp = &_face->glyph->bitmap;

	// Copy the raw bitmap data
	g->width = bmp->width;
	g->height = bmp->rows;
	g->data.resize(g->width*g->height);

	// Build the glyph data
	float *dp = g->data;

	for(int y = 0; y<g->height; y++)
	{
		uint8_t *sp = bmp->buffer+y*bmp->pitch;

		for(int x = 0; x<g->width; x++)
			*(dp++) = (float)sp[x] / 255.0f;
	}
		
	// Fill in the required metrics
	FT_Glyph_Metrics *met = &_face->glyph->metrics;

	g->metrics.width = met->width/64;
	g->metrics.height = met->height/64;
	g->metrics.advance = met->horiAdvance/64;
	g->metrics.x_bearing = met->horiBearingX/64;
	g->metrics.y_bearing = (_face->size->metrics.ascender-met->horiBearingY)/64;

	return g;
}
	
void Font::draw_glyph(Texture *tex, int dx, int dy, const Color& col, Glyph *g)
{
	// Clip
	Rect r;
	int ox, oy;
	if(!Rect::clip(Rect(dx, dy, g->width, g->height), tex->get_clip(), r, &ox, &oy))
		return;

	float *ptr = g->data + oy*g->width + ox;
	dx += ox;
	dy += oy;

	for(int y = 0; y<r.h; y++)
	{
		for(int x = 0; x<r.w; x++)
			tex->set_pixel_fast(dx+x, dy, Color::mix(tex->get_pixel_fast(dx+x, dy), col, ptr[x]));
	
		ptr += g->width;
		dy++;
	}
}

int Font::print(Texture *tex, int x, int y, const Color& col, const Str& text, bool kerning)
{
	Glyph *last = NULL;
	int ox = x;

	int pos = 0;
	for(;;)
	{
		// Get an UTF8 character
		int l;
		uint32_t ch = text.get_utf8_char(pos, l);
		if(!ch)
			break;

		pos += l;

		Glyph *g = get_glyph(ch);

		// Apply kerning
		if(kerning && FT_HAS_KERNING(_face) && last)
		{
			FT_Vector delta;
			FT_Get_Kerning(_face, last->index, g->index, FT_KERNING_DEFAULT, &delta);
			x += delta.x/64;
		}

		if(tex)
			draw_glyph(tex, x+g->metrics.x_bearing, y+g->metrics.y_bearing, col, g);

		x += g->metrics.advance;

		// Remember the glyph, for kerning
		last = g;
	}

	if(tex)
		tex->set_dirty();

	return x-ox;
}



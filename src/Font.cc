#include "pix5.h"
#include <ft2build.h>
#include FT_FREETYPE_H



namespace PIX {

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

	
	
Font::Font(const Str& fname, int ptsize)
{
	// Try to load the face
	Log::log("Loading font: %s", fname.ptr());

	reset();

	if(FT_New_Face(_ft, fname, 0, &_face))
		E::FreeType("Error loading font: %s", fname.ptr());

	setup(ptsize);
}
	
Font::Font(const void *buf, int size, int ptsize)
{
	// Try to load the face
	Log::log("Loading memory font");

	reset();

	if(FT_New_Memory_Face(_ft, (const FT_Byte*)buf, size, 0, &_face))
		E::FreeType("Error loading a memory font");

	setup(ptsize);
}

Font::~Font()
{
	// Free the glyphs
	if(_glyphs)
	{
		for(int c = 0; c<_face->num_glyphs; c++)
			if(_glyphs[c])
				delete _glyphs[c];

		free(_glyphs);
	}

	// Free the face
	if(_face)
		FT_Done_Face(_face);
}
	
void Font::reset()
{
	_face = NULL;
	_glyphs = NULL;
}
	
void Font::setup(int ptsize)
{
	// Set the size
	if(FT_Set_Char_Size(_face, 0, ptsize*64, 96, 96))
		E::FreeType("Error setting the font size to %i points", ptsize);

	#ifdef DBG
	Log::debug("Success. %i glyphs, %i sizes, %i fixed sizes", _face->num_glyphs, _face->available_sizes, _face->num_fixed_sizes);
	#endif

	// Allocate the glyphs buffers
	_glyphs = (Glyph**)calloc(_face->num_glyphs, sizeof(Glyph*));
}

Glyph *Font::get_glyph(int ch)
{
	// Get the glyph index for the given character
	int i = FT_Get_Char_Index(_face, ch);

	ASSERT(i>=0 && i<_face->num_glyphs, "FT_Get_Char_Index() returned an out-of-range index, this shouldn't happen")

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
	g->data = (uint8_t*)malloc(g->width*g->height);

	// Copy row by row, as we don't use a pitch whereas freetype uses one
	for(int y = 0; y<g->height; y++)
		memcpy(g->data+y*g->width, bmp->buffer+y*bmp->pitch, g->width);
		
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
	uint8_t *ptr = g->data;

	for(int y = 0; y<g->height; y++)
		for(int x = 0; x<g->width; x++)
			tex->set_pixel_fast(dx+x, dy+y, Color::mix(tex->get_pixel_fast(dx+x, dy+y), col, 255-*(ptr++)));
}

void Font::print(Texture *tex, int x, int y, const Color& col, const Str& text)
{
	Glyph *last = NULL;

	for(int c = 0; c<text.len(); c++)
	{
		Glyph *g = get_glyph(text[c]);

		draw_glyph(tex, x+g->metrics.x_bearing, y+g->metrics.y_bearing, col, g);
		x += g->metrics.advance;

		// Apply kerning
		if(FT_HAS_KERNING(_face) && last)
		{
			FT_Vector delta;
			FT_Get_Kerning(_face, last->index, g->index, FT_KERNING_DEFAULT, &delta);
			x += delta.x/64;
		}

		// Remember the glyph, for kerning
		last = g;
	}

	tex->set_dirty();
}


}

#pragma once


class Font
{
	struct FT_FaceRec_* _face;
	// The actual Freetype face

	Glyph **_glyphs;
	// All the glyphs

	void setup(int ptsize);
	// Setup the font once the face was loaded

	void reset();

	void draw_glyph(class Texture *tex, int dx, int dy, const Color& col, Glyph *g);
	// Draw a glyphr

public:

	Font(const Str& fname, int ptsize);
	// Load a new font file
	// 'fname' is the name of a .TTF file, 'ptsize' is the requested font size, in points

	Font(const void *buf, int size, int ptsize);
	// Load a new font from memory
	// 'buf' points to a memory buffer of 'size' bytes containing the equivalent of a TTF font file

	~Font();

	Glyph *get_glyph(int ch);
	// Get the glyph for the given unicode character
	// Per freetype convention, if the font doesn't have a glyph for the given
	// character, the first glyph (index 0) is returned, which is usually a square mark

	void print(class Texture *tex, int x, int y, const Color& col, const Str& text);
	// Print a string on the given texture
};



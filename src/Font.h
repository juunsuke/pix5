#pragma once


class Font
{
	struct FT_FaceRec_* _face;
	// The actual Freetype face

	Glyph **_glyphs;
	// All the glyphs

	int _height, _spacing;
	// Font height and vertical spacing


	Font();

	void setup(struct FT_FaceRec_* face, int ptsize);
	// Setup the font once the face was loaded

	void draw_glyph(class Texture *tex, int dx, int dy, const Color& col, Glyph *g, BlendMode::Type blend);
	// Draw a glyphr

public:

	static Font *load(const Str& fname, int ptsize);
	// Load a new font file
	// 'fname' is the name of a .TTF file, 'ptsize' is the requested font size, in points

	static Font *load(const void *buf, int size, int ptsize);
	// Load a new font from memory
	// 'buf' points to a memory buffer of 'size' bytes containing the equivalent of a TTF font file

	~Font();

	Glyph *get_glyph(int ch);
	// Get the glyph for the given unicode character
	// Per freetype convention, if the font doesn't have a glyph for the given
	// character, the first glyph (index 0) is returned, which is usually a square mark

	int print(class Texture *tex, int x, int y, const Color& col, const Str& text, BlendMode::Type blend = BlendMode::DestAlpha);
	// Print a string on the given texture
	// Returns the width, in pixels, used by the string

	inline int len(const Str& text) { return print(NULL, 0, 0, 0, text); }
	// Return the width, in pixels, required to draw the given string

	inline int height() { return _height; }
	// Return the height of the tallest character in the font

	inline int spacing() { return _spacing; }
	// Return the reccomended vertical spacing between lines of text
};



#pragma once


class TFState: public Obj
{
	PIXOBJ("TFState")

public:

	Font *font;
	// Current font

	Color fg_col;
	Color bg_col;
	// Foreground and background color

	TextAlign::Type align;
	// Text alignment

	VertTextAlign::Type valign;
	// Vertical text aligning, within the spacing

	int vspacing;
	// Vertical spacing


	TFState()
	{
	}

	TFState(const TFState& o)
	{
		// Copy constructor
		copy(o);
	}

	~TFState()
	{
	}

	void copy(const TFState& o)
	{
		// Copy another state
		font = o.font;
		fg_col = o.fg_col;
		bg_col = o.bg_col;
		align = o.align;
		valign = o.valign;
		vspacing = o.vspacing;
	}

	TFState& operator=(const TFState& o)
	{
		// Affectation
		copy(o);
		return *this;
	}
};


class TextFormat: public Obj
{
	PIXOBJ("TextFormat")

	TFState _state;
	// Current state

	List<class TFElement*> _elements;
	// All the elements

	List<class TFLine*> _lines;
	// Calculated lines

	int _cw, _ch;
	// Calculated widht and height


	void copy(const TextFormat& o);
	// Copy another format

	void add_word(class TFElement *el, int *px, int *py, int w, const Str& text);
	// Add a word to the current line
	
	void calc_text(class TFElement *el, int *px, int *py, int width);
	// Calc a text element's positioning

	void calc_texture(class TFElement *el, int *px, int *py, int width);
	// Calc a texture element's positioning

	void calc_newline(int *px, int *py);
	// Add a new line

	void add_align_point(int x, class TFLine *line = NULL);
	// Add a new align point in a line

	void align_line(class TFLine *line);
	// Perform alignments on a line

public:

	TextFormat();
	TextFormat(const TextFormat& o) { copy(o); }
	~TextFormat();

	void reset();
	// Clear the elements list and reset the default state

	void reset_elements();
	// Clear the elements, but retain the current state

	void reset_calc();
	// Reset the positioning calculations in elements

	TextFormat& operator=(const TextFormat& o)
	{
		// Affectation
		// Calculations will NOT carry over, only the definition
		reset();
		copy(o);
		return *this;
	}


	//
	// State manipilation
	// These functions change the current state of the format
	// The next call to add_text() will use this state
	//

	void set_font(Font *font = NULL);
	// Set the current font
	// If 'font' is NULL, the default built-in font will be set

	inline void set_color(const Color& col = Color(0, 0, 0))				{ _state.fg_col = col; }
	// Set the foreground color

	inline void set_bg_color(const Color& col = 0)
	{
		// Set the background color
		_state.bg_col = col;
	}

	inline void set_align(TextAlign::Type align = TextAlign::Left)			{ _state.align = align; }
	inline void set_valign(VertTextAlign::Type align = VertTextAlign::Top)	{ _state.valign = align; }
	// Set text alignment

	inline void set_vspacing(int vspacing = 0)								{ _state.vspacing = vspacing; }
	// Set vertical spacing
	// This is the height for one line, text will be aligned according
	// to valign within this height, and this amount of pixels will be
	// between the top of each line rect
	// Setting it to 0 uses the font's spacing value
	
	void reset_state()
	{
		set_font(NULL);
		set_color();
		set_bg_color();
		set_align();
		set_valign();
		set_vspacing();
	}


	//
	// Elements addition
	// These functions use the current state and add new elements to the format
	//

	int add_text(const Str& text);
	// Add a text element
	// Return a reference index, which can be used post-rendering to get positioning info

	inline void add_newline() { add_text("\n"); }
	// Add a line skip

	void set_x(int x);
	// Force an X position for the next element
	// Changing the X position this will will create an alignment point at 'x', so any
	// elements before or after the call to set_x() will use this coordinate as an
	// alignment bound

	void add_x(int x);
	// Add X pixels between the previous element and the next

	int add_tex(Texture *tex, const Rect& r);
	inline int add_tex(Texture *tex, int x, int y, int w, int h) { return add_tex(tex, Rect(x, y, w, h)); }
	inline int add_tex(Texture *tex) { return add_tex(tex, Rect(0, 0, tex->width(), tex->height())); }
	// Add the specified sub-section of a texture (or a whole texture) as a new element
	// The texture acts as a word of text and follows all states except the font
	// The texture is not copied, and must remain valid until the TextFormat is rendered


	//
	// Rendering and positioning
	//

	void calc(int width = 0);
	// Calculate the positioning of every element
	// If 'width' is greater than 0, a maximum width will be imposed and cause
	// word wrapping and alignment
	// If 'width' is 0, there is no maximum width, and as much as needed will be used
	// With no max width, alignment will only have an effect if there are more than
	// one line generated

	void render(Texture *tex);
	// Render to the given texture
	// Positioning must have been calculated alread with a call to calc(), or nothing
	// will be rendered

	Texture *build_tex(int width = 0, int height = 0);
	// Recalculate the positioning of elements and render them into a created texture
	// There is no need to call calc(), as this function will do it automatically

	inline int get_width() { return _cw; }
	inline int get_height() { return _ch; }
	// Get the total width/height required to hold everything
	// Must be called after calculating

	List<Rect> get_pos(int i);
	// Get a list of rectangles specifying the position of the element with index 'i'
	// This must be done after calculations have been performed
	// 'i' is the index as returned by add_text()
};


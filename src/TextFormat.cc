#include "pix5.h"
#include "data.h"

namespace PIX {


namespace ElementType
{
	enum Type
	{
		Text,
		SetX,
		AddX,
		Texture,
	};
}


class TFAlignPoint
{
public:

	int i;
	// Word index, within the line, that the align point precedes

	int x;
	// X position of the align point
};

class TFWord
{
public:

	Rect r;
	// Word position and size

	Str text;
	// Word text

	Texture *tex;
	Rect tex_rect;
	// Texture, for texture words

	class TFElement *el;
	// The element the word is part of


	TFWord()
	{
		tex = NULL;
		el = NULL;
	}

	~TFWord()
	{
	}
};

class TFLine
{
public:

	List<TFWord*> words;
	// Words in the line

	List<TFAlignPoint*> apoints;
	// Aling points of the line

	int h;
	// Line height


	TFLine() { }
	~TFLine()
	{
		words.clear_del();
		apoints.clear_del();
	}
};


class TFElement
{
public:

	TFState state;
	// The state for this element

	ElementType::Type type;
	// Element type

	Str text;
	// Text, for text element

	int val;
	// Value, for various elements:
	// SetX
	// AddX

	Texture *tex;
	Rect tex_rect;
	// Texture and its sub-section


	List<TFWord*> words;
	// All the calculated words


	TFElement(const TFState& state, ElementType::Type type)
	{
		this->state = state;
		this->type = type;
	}

	~TFElement()
	{
		words.clear_nodel();
	}
};



TextFormat::TextFormat()
{
	// Default state
	reset_state();
	_cw = 0;
	_ch = 0;
}

TextFormat::~TextFormat()
{
	reset_elements();
}

void TextFormat::reset()
{
	// Reset everything
	reset_elements();
	reset_state();
}

void TextFormat::reset_elements()
{
	// Delete all the elements
	_elements.clear_del();
	_lines.clear_del();
	_cw = 0;
	_ch = 0;
}
	
void TextFormat::reset_calc()
{
	// Reset calculations
	_lines.clear_del();

	for(int c = 0; c<_elements.size(); c++)
		_elements[c]->words.clear_nodel();
}

void TextFormat::set_font(Font *font)
{
	// Set the current font
	if(font)
		_state.font = font;
	else
		_state.font = Cache::font(data_LiberationSans_Regular_ttf, data_LiberationSans_Regular_ttf_len, 12);
}



int TextFormat::add_text(const Str& text)
{
	// Create a text element
	TFElement *el = new TFElement(_state, ElementType::Text);
	el->text = text;

	return _elements.add(el);
}

void TextFormat::set_x(int x)
{
	// Create a SetX element
	TFElement *el = new TFElement(_state, ElementType::SetX);
	el->val = x;
	_elements.add(el);
}

void TextFormat::add_x(int x)
{
	// Create an AddX element
	TFElement *el = new TFElement(_state, ElementType::AddX);
	el->val = x;
	_elements.add(el);
}
	
int TextFormat::add_tex(Texture *tex, const Rect& r)
{
	// Create a texture element
	TFElement *el = new TFElement(_state, ElementType::Texture);
	el->tex = tex;
	el->tex_rect = r;

	return _elements.add(el);
}



void TextFormat::add_word(TFElement *el, int *px, int *py, int w, const Str& text)
{
	// Create a new word
	TFWord *wd = new TFWord();
	wd->r = Rect(*px, *py, w, (el->state.vspacing>0 ? el->state.vspacing : el->state.font->spacing()));
	wd->text = text;
	wd->el = el;

	// Add a word to the current line
	_lines[_lines.size()-1]->words.add(wd);

	// Add it to the element also
	el->words.add(wd);

	// Adjust the pen
	*px += w;
}

void TextFormat::calc_text(class TFElement *el, int *px, int *py, int width)
{
	// Divide the text in words
	int start = 0;
	int len = el->text.len();

	// First get the size of a space
	int space_size = el->state.font->get_glyph(' ')->metrics.advance;

	// Look for a word cuts (space) or a new line characters (\n)
	for(int c = 0; c<=len; c++)
	{
		char ch = el->text[c];

		if(ch==' ' || ch=='\n' || ch==0)
		{
			// There is a splitting point
			// Add a word if needed
			if(c>start)
			{
				// Word text
				Str s = el->text.sub(start, c-start);
				int w = el->state.font->len(s);

				// Add a new line if it doesn't fit
				if(width>0 && (*px) && (*px+w)>width)
					calc_newline(px, py);

				// Add the word
				add_word(el, px, py, w, s);
			}

			// Add room for a space character, or change line
			if(ch==' ')
				*px += space_size;
			else if(ch=='\n')
				calc_newline(px, py);
		
			start = c+1;
		}
	}
}
	
void TextFormat::calc_texture(class TFElement *el, int *px, int *py, int width)
{
	// Add a new line ?
	if(width>0 && (*px) && (*px+el->tex_rect.w)>width)
		calc_newline(px, py);

	// Add a word for the texture
	TFWord *wd = new TFWord();
	wd->r = Rect(*px, *py, el->tex_rect.w, el->tex_rect.h);
	wd->tex = el->tex;
	wd->tex_rect = el->tex_rect;
	wd->el = el;

	// Add a word to the current line
	_lines[_lines.size()-1]->words.add(wd);

	// Add it to the element also
	el->words.add(wd);

	// Adjust the pen
	*px += el->tex_rect.w;
}

void TextFormat::calc_newline(int *px, int *py)
{
	int h = 0;

	// Find the last added word's state spacing
	for(int c = _lines.size()-1; c>=0; c--)
	{
		TFLine *line = _lines[c];

		if(line->words.size())
		{
			h = line->words[line->words.size()-1]->el->state.vspacing;
			break;
		}
	}

	if(!h)
	{
		// No words yet, use the spacing of the first element
		if(_elements.size())
		{
			int s = _elements[0]->state.vspacing;
			h = s ? s : _state.font->spacing();
		}
	}

	// Find the tallest word in the last line
	TFLine *line = _lines[_lines.size()-1];
	for(int c = 0; c<line->words.size(); c++)
	{
		TFWord *wd = line->words[c];

		if(wd->r.h>h)
			h = wd->r.h;
	}

	// Mark the line's height
	line->h = h;

	// Adjust the pen
	*px = 0;
	*py += h;

	// Add a new line
	_lines.add(new TFLine());

	// Add the initial align point
	add_align_point(0);
}
	
void TextFormat::add_align_point(int x, TFLine *line)
{
	// Get the last line if one is not provided
	if(!line)
		line = _lines[_lines.size()-1];

	// Create an align point for the line
	TFAlignPoint *ap = new TFAlignPoint();
	ap->i = line->words.size();
	ap->x = x;

	// Add it
	line->apoints.add(ap);
}
	
void TextFormat::align_line(class TFLine *line)
{
	// Perform alignment between each align point pairs
	for(int c = 0; c<line->apoints.size()-1; c++)
	{
		TFAlignPoint *ap1 = line->apoints[c];
		TFAlignPoint *ap2 = line->apoints[c+1];

		if(ap1->i==ap2->i)
			continue;

		// Perform horizontal alignment ?
		// Get the alignment of the element of the first word
		TextAlign::Type align = line->words[ap1->i]->el->state.align;

		// Calc the width span between the two align points
		int apw = ap2->x-ap1->x-1;

		// Calc the width of the words between the align points
		int x1 = line->words[ap1->i]->r.x;
		int ww = (line->words[ap2->i-1]->r.x + line->words[ap2->i-1]->r.w) - x1 - 1;
		int off = apw-ww;

		if(off>0)
		{
			if(align==TextAlign::Right || align==TextAlign::Center)
			{
				// Align and center are similar, an offset must be added to each word
				if(align==TextAlign::Center)
					off /= 2;

				// Adjust each word
				for(int d = ap1->i; d<ap2->i; d++)
					line->words[d]->r.x += off;
			}

			else if(align==TextAlign::Justify && (ap2->i-ap1->i)>1)
			{
				// Justify the text, distributing the extra space as evenly as possible between all the words
				int num = ap2->i-ap1->i-1;
				float off_per = (float)off / (float)num;
				float off_tot = 0;

				for(int d = ap1->i+1; d<ap2->i; d++)
				{
					if(d==(ap2->i-1))
						off_tot = off;
					else
						off_tot += off_per;

					line->words[d]->r.x += (int)off_tot;
				}
			}
		}
	}

	// Perform vertical alignments
	for(int c = 0; c<line->words.size(); c++)
	{
		TFWord *wd = line->words[c];

		int off = line->h - wd->r.h;

		switch(wd->el->state.valign)
		{
			case VertTextAlign::Top:
				// Do nothing
				break;

			case VertTextAlign::Bottom:
				// Bring to bottom
				wd->r.y += off;
				break;

			case VertTextAlign::Middle:
				// Center vertically
				wd->r.y += off/2;
				break;
		}
	}
}

void TextFormat::calc(int width)
{
	// Reset any current calculation
	reset_calc();
	
	// Create the first line and its align point
	_lines.add(new TFLine());
	add_align_point(0);

	// Calculate the positionning of every element
	int px = 0;
	int py = 0;

	for(int c = 0; c<_elements.size(); c++)
	{
		TFElement *el = _elements[c];

		// Some state changes require the insertion of an align point
		if(c)
		{
			TFState& s1 = _elements[c-1]->state;
			TFState& s2 = el->state;

			if(s1.align!=s2.align)
				add_align_point(px);
		}

		switch(el->type)
		{
			case ElementType::Text:
				calc_text(el, &px, &py, width);
				break;

			case ElementType::SetX:
				// Change px
				px = el->val;

				// Add an align point
				add_align_point(px);

				break;

			case ElementType::AddX:
				// Add a value to px
				px += el->val;
				break;

			case ElementType::Texture:
				calc_texture(el, &px, &py, width);
				break;
		}

		// Adjust the pen if needed
		if(width>0 && px>=width)
			calc_newline(&px, &py);
	}

	// If no width was given, find out the maximum width
	if(width<=0)
	{
		for(int c = 0; c<_lines.size(); c++)
		{
			TFLine *line = _lines[c];

			if(line->words.size())
			{
				TFWord *wd = line->words[line->words.size()-1];

				if((wd->r.x+wd->r.w)>width)
					width = wd->r.x+wd->r.w;
			}
		}
	}

	// Add a final align point to each line and align them
	for(int c = 0; c<_lines.size(); c++)
	{
		TFLine *line = _lines[c];

		// Add the final align point
		add_align_point(width, line);

		// Align the line
		align_line(line);
	}

	// If there is no empty line at the end, add one for proper height calculation
	if(!_lines.size() || _lines[_lines.size()-1]->words.size())
		calc_newline(&px, &py);

	// Mark the total size
	_cw = width;
	_ch = py;
}





void TextFormat::render(Texture *tex)
{
	// Render all the words
	for(int c = 0; c<_elements.size(); c++)
	{
		TFElement *el = _elements[c];

		for(int d = 0; d<el->words.size(); d++)
		{
			TFWord *wd = el->words[d];
	
			// Background
			tex->rect_fill(wd->r, el->state.bg_col);

			if(wd->tex)
			{
				// Texture word
				tex->blit(wd->r.x, wd->r.y, wd->tex, wd->tex_rect);
			}
			else
			{
				// Text word
				tex->print(el->state.font, wd->r.x, wd->r.y, el->state.fg_col, wd->text, true, el->state.blend);
			}
		}
	}
}

Texture *TextFormat::build_tex(int width, int height)
{
	// Calc positioning
	calc(width);

	// Create a texture
	Texture *tex = Texture::create((width>0) ? width : get_width(), (height>0) ? height : get_height());
	render(tex);

	return tex;
}

List<Rect> TextFormat::get_pos(int i)
{
	ASSERT(i>=0 && i<_elements.size(), "TextFormat::get_pos(): Invalid index")

	// Build a list of rects for words
	List<Rect> lr;

	// Check all the words of the element
	TFElement *el = _elements[i];
	for(int c = 0; c<el->words.size(); c++)
		lr.add(el->words[c]->r);

	return lr;
}

}

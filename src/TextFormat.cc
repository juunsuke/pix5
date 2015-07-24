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
	};
}

class TFWord
{
public:

	Rect r;
	// Word position and size

	Str text;
	// Word text

	class TFElement *el;
	// The element the word is part of
};

class TFLine
{
public:

	List<TFWord*> words;
	// Words in the line

	TFLine() { }
	~TFLine()
	{
		words.clear_del();
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
	
void TextFormat::add_words(class TFElement *el, int *px, int *py, const Str& text)
{
	// Split the string into space-separated words and add them individually
	int len = text.len();

	if(!len)
		return;

	// First get the size of a space
	int space_size = el->state.font->get_glyph(' ')->metrics.advance;

	// Split in words
	int start = 0;
	int pos = text.find(' ');

	if(pos==-1)
		pos = len;

	for(;;)
	{
		if(pos>start)
		{
			// Add a word
			Str w = text.sub(start, pos-start);
			add_word(el, px, py, el->state.font->len(w), w);
		}

		// Add a space
		*px += space_size;

		// Move on
		start = pos+1;

		// Done ?
		if(start>=len)
			break;

		pos = text.find(' ', start);
		if(pos==-1)
			pos = len;
	}
}

void TextFormat::calc_text(class TFElement *el, int *px, int *py, int width)
{
	// Simplest case: No line changes (\n) and unlimited width
	int pos = el->text.find('\n');
	if(pos==-1 && width<=0)
	{
		// Super simple, add a single word
		add_words(el, px, py, el->text);
		return;
	}

	// There will be line changes one way or another
	Str cur, remain;

	bool force_nl = false;
	bool force_last_nl = false;

	if(pos==-1)
	{
		// No manual line skip
		cur = el->text;
	}
	else
	{
		// Manual skip
		cur = el->text.left(pos);
		remain = el->text.sub(pos+1);
		force_nl = true;
	}

	// Process the text
	while(cur.len() || remain.len() || force_nl)
	{
		if(cur.len())
		{
			// There is a section to process
			// Does it fit ?
			int curlen = el->state.font->len(cur);
			if(width<=0 || (*px+curlen)<=width)
			{
				// Yes, create a word for it
				add_words(el, px, py, cur);
				cur = "";
			}
			else
			{
				// The whole of cur doesn't fit
				// It may have to be split

				// Find the farthest split point
				bool was_cut = false;

				for(int c = cur.len()-1; c>0; c--)
				{
					char ch = cur[c];

					//if(!((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9') || ch=='_'))
					if(ch==' ')
					{
						Str part = cur.left(c);
						int len = el->state.font->len(part);

						if((*px+len)<=width)
						{
							// It fits
							// Add this part as a word
							add_words(el, px, py, part);

							// Set the remains, skipping leading spaces
							while(c<cur.len() && cur[c]==' ')
								c++;

							cur = cur.sub(c);
							was_cut = true;
							break;
						}
					}
				}

				// If there was no cut, the whole thing is a non-breakable word
				// Add it as a word, can't be helped
				if(!was_cut)
				{
					add_words(el, px, py, cur);
					cur = "";
				}
			}
		}

		// Done ?
		if(!force_nl && !cur.len() && !remain.len())
			break;

		force_nl = false;

		// We must skip a line at this point
		calc_newline(px, py);

		// If there's still something left in cur, start over
		if(cur.len())
			continue;

		// Check for manual new lines
		pos = remain.find('\n');
		if(pos==-1)
		{
			// No more
			cur = remain;
			remain = "";
		}
		else
		{
			// Cut
			cur = remain.left(pos);
			remain = remain.sub(pos+1);
			force_nl = true;

			// Special case: If remain is now empty, force a last newline
			if(cur.len() && !remain.len())
				force_last_nl = true;
		}
	}

	// Forced last new line ?
	if(force_last_nl)
		calc_newline(px, py);
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

	// Adjust the pen
	*px = 0;
	*py += h;

	// Add a new line
	_lines.add(new TFLine());
}

void TextFormat::calc(int width)
{
	// Reset any current calculation
	reset_calc();
	
	// Create the first line
	_lines.add(new TFLine());

	// Calculate the positionning of every element
	int px = 0;
	int py = 0;

	for(int c = 0; c<_elements.size(); c++)
	{
		TFElement *el = _elements[c];

		switch(el->type)
		{
			case ElementType::Text:
				calc_text(el, &px, &py, width);
				break;

			case ElementType::SetX:
				// Change px
				px = el->val;
				break;

			case ElementType::AddX:
				// Add a value to px
				px += el->val;
				break;
		}

		// Adjust the pen if needed
		if(width>0 && px>=width)
			calc_newline(&px, &py);
	}
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
	
			tex->rect_fill(wd->r, el->state.bg_col);
			tex->print(el->state.font, wd->r.x, wd->r.y, el->state.fg_col, wd->text, true, el->state.blend);
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

int TextFormat::get_width()
{
	return 0;
}

int TextFormat::get_height()
{
	return 0;
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

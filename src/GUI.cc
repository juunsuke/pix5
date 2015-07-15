#include "pix5.h"
#include "data.h"

namespace PIX {
namespace GUI {


Font *font_default = NULL;
// Default fonts

static TexRenderer _tr;
// The TexRenderer for the GUI

static GuiElement *_root = NULL;
// Root element

static GuiTheme *_def_theme = NULL;
// The default theme


void init()
{
	// Create the root element
	_root = new GuiElement("Root", false, 0, 0);

	// Default fonts
	font_default = Cache::font(data_LiberationSans_Regular_ttf, data_LiberationSans_Regular_ttf_len, 12);

	// Create the default theme
	_def_theme = new GuiTheme();
}

void done()
{
	if(_root)
	{
		delete _root;
		_root = NULL;
	}

	if(_def_theme)
	{
		delete _def_theme;
		_def_theme = NULL;
	}

	font_default = NULL;

	_tr.clear();
}

void add_child(GuiElement *el, int x, int y)
{
	_root->add_child(el, x, y);
}

static void draw(GuiElement *el, int bx, int by)
{
	// Stop here if the element is not visible
	if(!el->visible())
		return;

	// Draw this element if it's a renderable
	if(el->render())
	{
		// Redraw the texture ?
		if(el->dirty())
		{
			el->on_draw(el->tex());
			el->set_dirty(false);
		}

		_tr.add(el->tex(), bx+el->x(), by+el->y());
	}

	// Do the children
	for(int c = 0; c<el->num_child(); c++)
		draw(el->get_child(c), bx+el->x(), by+el->y());
}

void draw(int ox, int oy)
{
	// Add each element to the TexRenderer
	draw(_root, ox, oy);

	// Do the actual drawing
	_tr.draw();
}

GuiTheme *default_theme()
{
	return _def_theme;
}




}}

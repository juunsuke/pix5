#include "pix5.h"

namespace PIX {


GuiElement::GuiElement(const Str& name, bool render, int w, int h)
{
	_name = name;
	_render = render;
	_parent = NULL;
	_x = 0;
	_y = 0;
	_w = w;
	_h = h;
	_visible = true;
	_dirty = true;
	_theme = GUI::default_theme();

	if(render)
	{
		ASSERT(w>0 && h>0, "GuiElement::GuiElement(): Renderable elements must have positive dimensions")

		_tex = Texture::create(w, h);
	}
	else
		_tex = NULL;
}

GuiElement::~GuiElement()
{
	if(_tex)
		delete _tex;

	// Remove all children
	_child.clear_del();
}
	
void GuiElement::resize(int w, int h)
{
	if(w==_w && h==_h)
		return;

	if(_render)
	{
		ASSERT(w>0 && h>0, "GuiElement::resize(): Renderable elements must have position dimensions")

		// Redo the texture
		delete _tex;
		_tex = Texture::create(w, h);
	}

	_w = w;
	_h = h;
}

void GuiElement::set_visible(bool vis)
{
	if(_visible==vis)
		return;

	_visible = vis;
}

void GuiElement::add_child(GuiElement *el, int x, int y)
{
	if(el->_parent)
		E::HasParent("GuiElement::add_child(): The new child already has a parent");

	// Add it to our children list
	el->_x = x;
	el->_y = y;
	_child.add(el);
}

void GuiElement::remove_child(GuiElement *el)
{
	// Look for the child
	for(int c = 0; c<_child.size(); c++)
		if(_child[c]==el)
		{
			// Found it
			// Remove it
			_child.remove_nodel(c);

			return;
		}

	// Not found
	E::NoChild("GuiElement::remove_child(): Child not found");
}

void GuiElement::remove()
{
	if(!_parent)
		E::NoParent("GuiElement::remove(): The element does not have a prent");

	// Let the parent do the removal
	_parent->remove_child(this);
}



}

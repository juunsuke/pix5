#pragma once


class GuiElement
{
	Str _name;
	// Element name

	bool _render;
	// Wether the element is renderable

	GuiElement *_parent;
	// Hierarchical parent

	int _x, _y;
	// Element position, relative to its parent

	int _w, _h;
	// Element size

	Texture *_tex;
	// The texture, for renderable elements

	List<GuiElement*> _child;
	// All children elements

	bool _visible;
	// Wether the element is visible

	bool _dirty;
	// Dirty flag
	
	class GuiTheme *_theme;
	// The theme used by the element

public:

	GuiElement(const Str& name, bool render, int w, int h);
	virtual ~GuiElement();


	inline Str name() { return _name; }
	inline GuiElement *parent() { return _parent; }
	inline int x() { return _x; }
	inline int y() { return _y; }
	inline int width() { return _w; }
	inline int height() { return _h; }
	inline Texture *tex() { return _tex; }
	inline bool visible() { return _visible; }
	inline bool render() { return _render; }
	inline int num_child() { return _child.size(); }
	inline bool dirty() { return _dirty; }
	inline GuiTheme *theme() { return _theme; }
	// Retrieve info about the element
	
	inline GuiElement *get_child(int index)
	{
		// Get a child element
		ASSERT(index>=0 && index<_child.size(), "GuiElement::get_child(): Invalid child index")
		return _child[index];
	}

	inline void set_name(const Str& name) { _name = name; }
	// Change the element name

	inline void move(int x, int y) { _x = x; _y = y; }
	// Change the element's position, relative to its parent

	void resize(int w, int h);
	// Change the element size

	void set_visible(bool vis);
	inline void show() { set_visible(true); }
	inline void hide() { set_visible(false); }
	// Change visibility

	inline void set_dirty(bool dirty = true) { _dirty = dirty; }
	// Set the dirty flag

	void set_theme(GuiTheme *theme)
	{
		// Change the theme
		if(_theme!=theme)
		{
			_theme = theme;
			on_apply_theme(theme);
			set_dirty();
		}
	}


	void add_child(GuiElement *el, int x, int y);
	// Add a child element

	void remove_child(GuiElement *el);
	// Remove one of our children from the hierarchy
	// This will not delete the child element

	void remove();
	// Remove this element from its parent's hierarchy



	//
	// Virtual events
	//

	virtual void on_apply_theme(GuiTheme *theme) {}
	// Called when the theme has been changed

	virtual void on_draw(Texture *tex) {}
	// Redraw the element
};




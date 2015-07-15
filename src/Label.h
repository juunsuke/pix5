#pragma once


namespace ImageLocation
{
	enum Type
	{
		Top,
		Bottom,
		Left,
		Right
	};
}


class Label: public GuiElement
{
	Str _text;
	// Label text

	TextAlign::Type _align;
	// Alignment

	Texture *_image;
	// Image, if any, NULL if none

	ImageLocation::Type _image_loc;
	// Image location

public:

	Label(const Str& name, int w, int h, const Str& text, Texture *image = NULL, ImageLocation::Type image_loc = ImageLocation::Top);
	~Label();


	inline Str text() { return _text; }
	inline TextAlign::Type align() { return _align; }
	inline Texture *image() { return _image; }
	inline ImageLocation::Type image_location() { return _image_loc; }
	// Get info about the label

	void set_text(const Str& text)
	{
		_text = text;
		set_dirty();
	}

	void set_align(TextAlign::Type align)
	{
		_align = align;
		set_dirty();
	}

	void set_image(Texture *image, ImageLocation::Type image_loc = ImageLocation::Top)
	{
		_image = image;
		_image_loc = image_loc;
		set_dirty();
	}


	//
	// Events
	//
	void on_draw(Texture *tex);
};



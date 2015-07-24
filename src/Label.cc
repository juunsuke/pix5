#include "pix5.h"

namespace PIX {


Label::Label(const Str& name, int w, int h, const Str& text, Texture *image, ImageLocation::Type image_loc) : GuiElement(name, true, w, h)
{
	// Set the default values
	_text = text;
	_align = LabelAlign::MiddleCenter;
	_image = image;
	_image_loc = image_loc;
}

Label::~Label()
{
}

void Label::on_draw(Texture *tex)
{
	// Clear the back
	theme()->label_draw(this, tex);
}


}

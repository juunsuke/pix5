#pragma once

class Button: public GuiElement
{
public:

	Button(const Str& name, int w, int h, const Str& text);
	~Button();
};



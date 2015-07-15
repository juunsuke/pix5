#pragma once


namespace GUI
{


extern Font *font_default;
// Default font


void draw(int ox = 0, int oy = 0);
// Draw the whole GUI
// ox,oy are offset coordinates added to every element

void add_child(GuiElement *el, int x, int y);
// Add a child element to the GUI root

GuiTheme *default_theme();
// Get the default theme


}

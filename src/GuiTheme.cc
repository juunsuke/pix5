#include "pix5.h"

namespace PIX {

	
GuiTheme::GuiTheme()
{
	// Default theme values
	label.font = GUI::font_default;
	label.fg_col = Color(0, 0, 0);
	label.bg_col = Color(0, 0, 0, 0);

}

GuiTheme::~GuiTheme()
{
}


}

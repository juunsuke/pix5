#pragma once

class GuiTheme
{
public:

	//
	// Label
	//
	struct
	{
		Font *font;
		// Label font

		Color fg_col, bg_col;
		// Foreground/background color
	} label;

	virtual void label_calc_pos(Label *lab, int *tx, int *ty, int *ix, int *iy);
	// Calc the position of there the text (tx,ty) and image (ix,iy) should be drawn

	virtual void label_draw_back(Label *lab, Texture *tex);
	// Draw the background

	virtual void label_draw_text(Label *lab, Texture *tex, int x, int y);
	// Draw the text

	virtual void label_draw_image(Label *lab, Texture *tex, int x, int y);
	// Draw the image

	virtual void label_draw(Label *lab, Texture *tex);
	// Draw a label



	GuiTheme();
	virtual ~GuiTheme();

};


#include "pix5.h"

namespace PIX {



static void align(TextAlign::Type align, int tw, int th, int w, int h, int spacing, int *x, int *y)
{
	// Align the (w x h) rectangle using the current alignment
	switch(align)
	{
		case TextAlign::TopLeft:				*x = spacing;			*y = spacing;			break;
		case TextAlign::BottomLeft: 			*x = spacing;			*y = th-h-spacing;		break;
		case TextAlign::MiddleLeft: 			*x = spacing;			*y = (th-h)/2;			break;

		case TextAlign::TopRight:				*x = tw-w-spacing;		*y = spacing;			break;
		case TextAlign::BottomRight:			*x = tw-w-spacing;		*y = th-h-spacing;		break;
		case TextAlign::MiddleRight:			*x = tw-w-spacing;		*y = (th-h)/2;			break;

		case TextAlign::TopCenter:				*x = (tw-w)/2;			*y = spacing;			break;
		case TextAlign::BottomCenter:   		*x = (tw-w)/2;			*y = th-h-spacing;		break;
		case TextAlign::MiddleCenter:   		*x = (tw-w)/2;			*y = (th-h)/2;			break;
		//case TextAlign::MiddleCenter:   		*x = (tw-w)/2;			*y = (th-h-(h-ha))/2;		break;

		default:
			*x = 0;
			*y = 0;
			break;
	}
}

void GuiTheme::label_calc_pos(Label *lab, int *tx, int *ty, int *ix, int *iy)
{
	// Calc the position of the image and text
	Str text = lab->text();
	Texture *img = lab->image();

	if(text.len() && img)
	{
		// The label has both text and an image
		int iw = img->width();
		int ih = img->height();
		int tw = label.font->len(text);
		int th = label.font->height();

		// Build the bounding box
		int w, h;

		switch(lab->image_location())
		{
			case ImageLocation::Top:
			case ImageLocation::Bottom:
				// Image on top of or below the label
				h = ih + th + 5;

				if(iw>tw)
				{
					// Image is larger than text
					w = iw;
					*ix = 0;
					*tx = (iw-tw)/2;
				}
				else
				{
					// Text is larger than image
					w = tw;
					*tx = 0;
					*ix = (tw-iw)/2;
				}

				if(lab->image_location()==ImageLocation::Top)
				{
					// Image on top
					*iy = 0;
					*ty = ih+5;
				}
				else
				{
					// Text on top
					*ty = 0;
					*iy = th+5;
				}

				break;

			case ImageLocation::Left:
			case ImageLocation::Right:
				// Image on the left or right of the label
				w = iw + tw + 5;

				if(ih>th)
				{
					// Image is taller than text
					h = ih;
					*iy = 0;
					*ty = (ih-th)/2;
				}
				else
				{
					// Text is taller than image
					h = th;
					*ty = 0;
					*iy = (th-ih)/2;
				}

				if(lab->image_location()==ImageLocation::Left)
				{
					// Image on the left
					*ix = 0;
					*tx = iw+5;
				}
				else
				{
					// Text on the left
					*tx = 0;
					*ix = tw+5;
				}

				break;

			default:
				w = 0;
				h = 0;
				*ix = 0;
				*iy = 0;
				*tx = 0;
				*ty = 0;
				break;
		}

		// Align the box
		int x, y;
		align(lab->align(), lab->width(), lab->height(), w, h, 5, &x, &y);

		// Adjust all positions
		*ix += x;
		*iy += y;
		*tx += x;
		*ty += y;
	}
	else if(img)
	{
		// The label has an image, no text
		align(lab->align(), lab->width(), lab->height(), img->width(), img->height(), 5, ix, iy);
		*tx = 0;
		*ty = 0;
	}
	else if(text.len())
	{
		// Text only, no image
		align(lab->align(), lab->width(), lab->height(), label.font->len(text), label.font->height(), 5, tx, ty);
		*ix = 0;
		*iy = 0;
	}
	else
	{
		// Nothing to draw
		*ix = 0;
		*iy = 0;
		*tx = 0;
		*ty = 0;
	}
}

void GuiTheme::label_draw_back(Label *lab, Texture *tex)
{
	// Clear
	tex->clear(label.bg_col);
}

void GuiTheme::label_draw_text(Label *lab, Texture *tex, int x, int y)
{
	tex->print(label.font, x, y, label.fg_col, lab->text(), (uint32_t)label.bg_col==0 ? BlendMode::Copy : BlendMode::DestAlpha);
}

void GuiTheme::label_draw_image(Label *lab, Texture *tex, int x, int y)
{
	tex->blit(x, y, lab->image(), true);
}

void GuiTheme::label_draw(Label *lab, Texture *tex)
{
	// Clear the back
	label_draw_back(lab, tex);

	// Calc positions
	int tx, ty;
	int ix, iy;
	label_calc_pos(lab, &tx, &ty, &ix, &iy);

	// Draw text, image, or both
	if(lab->text().len())
		label_draw_text(lab, tex, tx, ty);
	
	if(lab->image())
		label_draw_image(lab, tex, ix, iy);
}



}

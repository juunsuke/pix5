#pragma once

class Glyph
{
public:

	int index;
	// Glyph index

	int width, height;
	// Glyph raw size, in pixels

	float *data;
	// Raw glyph data


	//
	// Glyph metrics
	//
	struct
	{
		int width, height;
		// Glyph bounding box

		int advance;
		// Horizontal spacing

		int x_bearing, y_bearing;
		// Bearings
		// Not the actual freetype bearing value, but rather converted to
		// assume the top is the baseline
	} metrics;
	


	Glyph();
	~Glyph();

};


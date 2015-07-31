#pragma once


class Rect: public Obj
{
	PIXOBJ("Rect")

public:

	int x, y;
	int w, h;
	// Rect parameters


	inline Rect()
	{
		// Empty rect
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}

	inline Rect(int x, int y, int w, int h)
	{
		// Provided parameters
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	inline Rect(int w, int h)
	{
		// Provided size only
		x = 0;
		y = 0;
		this->w = w;
		this->h = h;
	}

	inline Rect(const Rect& o)
	{
		// Copy constructor
		x = o.x;
		y = o.y;
		w = o.w;
		h = o.h;
	}

	inline Rect& operator=(const Rect& o)
	{
		// Affectation
		x = o.x;
		y = o.y;
		w = o.w;
		h = o.h;

		return *this;
	}

	inline ~Rect() { }


	inline bool contains(int px, int py) const
	{
		// Check wether the given point is contained in the rect
		return px>=x && px<(x+w) && py>=y && py<(y+h);
	}

	inline bool contains(const Rect& o) const
	{
		// Check wether this rect completely contains another rect
		return contains(o.x, o.y) && contains(o.x+o.w, o.y+o.h);
	}

	inline bool outside(const Rect &o) const
	{
		// Check wether the two rects don't touch at all
		return x>=(o.x+o.w) || (x+w)<=o.x || y>=(o.y+o.h) || (y+h)<=o.y;
	}

	

	static bool clip(const Rect& r, const Rect& bounds, Rect& res, int *dx = NULL, int *dy = NULL)
	{
		// Clip the rect 'r' to fit inside rect 'bounds'
		// If the rects don't touch, the function immediatly returns false
		// The resulting clipped rectangle will be stored in 'res'
		// If not NULL, *dx and *dy will be set to the amount that 'r' has been reduced on
		// its left and top size, respectivelt

		if(r.outside(bounds))
			return false;

		res = r;

		// Clip the left side
		if(res.x<bounds.x)
		{
			int d = bounds.x - res.x;
			res.x = bounds.x;
			res.w -= d;

			if(dx)
				*dx = d;
		}
		else if(dx)
			*dx = 0;

		// Clip the top side
		if(res.y<bounds.y)
		{
			int d = bounds.y - res.y;
			res.y = bounds.y;
			res.h -= d;

			if(dy)
				*dy = d;
		}
		else if(dy)
			*dy = 0;

		// Clip the right side
		if((res.x+res.w)>(bounds.x+bounds.w))
			res.w = bounds.x+bounds.w-res.x;

		// Clip the bottom side
		if((res.y+res.h)>(bounds.y+bounds.h))
			res.h = bounds.y+bounds.h-res.y;

		return true;
	}
};


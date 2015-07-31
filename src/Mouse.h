#pragma once


class Mouse: public Obj
{
	PIXOBJ("Mouse")

public:

	int x, y;
	// Mouse position

	uint32_t buts;
	// The state of mouse buttons, as a bitfield
	// 1 = The button is down
	// 0 = The button is up
	// 0x00000001 = Left mouse button
	// 0x00000002 = Middle mouse button
	// 0x00000004 = Right mouse button

	inline void reset()
	{
		x = 0;
		y = 0;
		buts = 0;
	}

	inline Mouse()
	{
		reset();
	}

	inline Mouse(const Mouse& o)
	{
		// Copy constructor
		x = o.x;
		y = o.y;
		buts = o.buts;
	}

	inline ~Mouse() { }

	inline Mouse& operator=(const Mouse& o)
	{
		// Affectation
		x = o.x;
		y = o.y;
		buts = o.buts;

		return *this;
	}

	inline bool button(int but) const
	{
		// Check a button state
		// but ranges from 1 to 32 inclusively
		// 1 = Left button
		// 2 = Middle button
		// 3 = Right button
		// 4 = X1
		// 5 = X2
		// The mouse back/forward buttons are sometimes mapped to 8 and 9
		// It seems to be system-dependant
		ASSERT(but>=1 && but<=32, "Mouse::button(): Invalid button index")
		return (buts&(1<<(but-1))) ? true : false;
	}

	inline bool operator[](int index) const
	{
		// Shortcut for button()
		ASSERT(index>=1 && index<=32, "Mouse::operator[]: Invalid button index")
		return button(index);
	}

	inline bool left() const { return button(1); }
	inline bool middle() const { return button(2); }
	inline bool right() const { return button(3); }
	// Return the state of the standard buttons
};


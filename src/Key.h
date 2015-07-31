#pragma once


class Key: public Obj
{
	PIXOBJ("Key")
public:

	KeyCode code;
	// Code/scancode for the key
	// This number, ranging from 0 to 511, represents the same 3 things:
	// - The PIX5 key code, as defined in "keys.h"
	// - The SDL2 scancode value
	// - The actual keybord scancode, as per USB specifications
	// This code does not take into account the user's keyboard layout, and thus
	// the key code names are based on the US keyboard layout
	// As such, this should not be used for text input, but rather for action buttons
	// If the code is 0, there is no key to represent

	int sdl_keycode;
	// SDL keycode for the key, which takes into account the user's keyboard layout
	// It's here for information, but shouldn't normally be needed

	Str code_name;
	// Scancode name, as provided by SDL
	// This is the name of the key, not taking into consideration the user's keyboard layout
	// It represents the name of the key on a US keyboard layout
	// It is provided for informational purposes, and its use should be avoided if possible

	Str key_name;
	// Key name, as provided by SDL
	// This is the name of the key, while taking into account the user's keyboard layout
	// When displaying to the user which key they pressed, this string should be used, as
	// it will represent what's written on their keyboard
	// For instance, on a french keyboard, the first alphabetic key on the top-left of the
	// keyboard is 'A', whereas on a US keyboard it is 'Q'.
	// For that key, key_name would be "a" and code_name would be "q"

	bool down;
	// Wether the key is down (true) or up (false)


	Key()
	{
		code = KEY_NONE;
		down = false;
	}

	Key(const Key& o)
	{
		// Copy constructor
		code = o.code;
		code_name = o.code_name;
		key_name = o.key_name;
		down = o.down;
	}

	~Key() {}

	inline Key& operator=(const Key& o)
	{
		// Affectation
		code = o.code;
		code_name = o.code_name;
		key_name = o.key_name;
		down = o.down;

		return *this;
	}

	inline Key& operator=(bool down)
	{
		// Set the down state
		this->down = down;
		return *this;
	}

	inline operator bool() const
	{
		return down;
	}

	inline bool operator==(KeyCode code) const
	{
		// Compare the codes
		return this->code==code;
	}
};


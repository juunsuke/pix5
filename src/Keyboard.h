#pragma once


//
// Keyboard mods bit fields
//
#define MOD_SHIFT				0x01
#define MOD_CTRL				0x02
#define MOD_ALT				0x04
#define MOD_GUI				0x08


class Keyboard
{
public:

	Key keys[512];
	// All the possible keys
	// Although it is public, it should be considered private and not used directly
	// Use helper functions instead

	void reset()
	{
		for(int c = 0; c<512; c++)
			keys[c].down = false;
	}

	Keyboard()
	{
		reset();
	}

	~Keyboard() { }


	inline Key& get_key(KeyCode key)
	{
		// Get a key
		ASSERT(key>=0 && key<512, "Keyboard::get_key(): Invalid key index")
		return keys[key];
	}

	inline Key& operator[](KeyCode key)
	{
		// Get a key
		ASSERT(key>=0 && key<512, "Keyboard::operator[]: Invalid key index")
		return keys[key];
	}

	inline bool is_down(KeyCode key)
	{
		// Check wether a key is down
		ASSERT(key>=0 && key<512, "Keyboard::is_down(): Invalid key index")
		return keys[key].down;
	}

	inline bool shift()	{ return keys[KEY_LSHIFT].down || keys[KEY_RSHIFT].down; }
	inline bool ctrl()	{ return keys[KEY_LCTRL].down  || keys[KEY_RCTRL].down; }
	inline bool alt()	{ return keys[KEY_LALT].down   || keys[KEY_RALT].down; }
	inline bool gui()	{ return keys[KEY_LGUI].down   || keys[KEY_RGUI].down; }

	int mods()
	{
		int mod = 0;

		if(shift())		mod |= MOD_SHIFT;
		if(ctrl())		mod |= MOD_CTRL;
		if(alt())		mod |= MOD_ALT;
		if(gui())		mod |= MOD_GUI;

		return mod;
	}
};


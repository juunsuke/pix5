#pragma once



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
};


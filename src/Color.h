#pragma once


class Color
{
	uint32_t _val;
	// Actual color

	inline uint32_t f2i(float v)
	{
		// Clamp and convert a float component to an integer value in range 0..255
		if(v<0)
			v = 0;
		else if(v>1.0f)
			v = 1.0f;

		return (uint32_t)(v*255.0f);
	}

public:

	inline Color()
	{
		// Empty
		_val = 0;
	}

	inline Color(const Color& col)
	{
		// Copy constructor
		_val = col._val;
	}

	inline Color(uint32_t val)
	{
		// Set the value
		_val = val;
	}

	inline Color(float r, float g, float b, float a = 1.0f)
	{
		// Build from float components
		seti(f2i(r), f2i(g), f2i(b), f2i(a));
	}

	inline void seti(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255)
	{
		// Directly set integer components, no clamping performed
		_val = ((a&0xFF)<<24) | ((b&0xFF)<<16) | ((g&0xFF)<<8) | (r&0xFF);
	}

	inline void setf(float r, float g, float b, float a = 1.0f)
	{
		// Set from float components, clamping is performed
		seti(f2i(r), f2i(g), f2i(b), f2i(a));
	}

	inline Color& operator=(const Color& o)
	{
		// Affectation
		_val = o._val;
		return *this;
	}

	inline Color& operator=(const uint32_t val)
	{
		// Affectation
		_val = val;
		return *this;
	}

	inline bool operator==(const Color& o) const
	{
		// Comparison
		return _val==o._val;
	}

	inline bool operator==(const uint32_t o) const
	{
		// Comparison
		return _val==o;
	}

	inline uint32_t get() const { return _val; }
	inline operator uint32_t() const { return _val; }
	// Get the color constant

	inline uint32_t ir() const { return _val&0xFF; }
	inline uint32_t ig() const { return (_val>>8)&0xFF; }
	inline uint32_t ib() const { return (_val>>16)&0xFF; }
	inline uint32_t ia() const { return (_val>>24)&0xFF; }
	// Get integer components

	inline float fr() const { return (float)ir()/255.0f; }
	inline float fg() const { return (float)ig()/255.0f; }
	inline float fb() const { return (float)ib()/255.0f; }
	inline float fa() const { return (float)ia()/255.0f; }
	// Get float components

	inline void ir(uint32_t r) { _val = (_val&0xFFFFFF00) | (r&0xFF); }
	inline void ig(uint32_t g) { _val = (_val&0xFFFF00FF) | ((g&0xFF)<<8); }
	inline void ib(uint32_t b) { _val = (_val&0xFF00FFFF) | ((b&0xFF)<<16); }
	inline void ia(uint32_t a) { _val = (_val&0x00FFFFFF) | ((a&0xFF)<<24); }
	// Change a single component of the color


	inline static Color ubyte(uint32_t r, uint32_t g, uint32_t b, uint32_t a=255)
	{
		return ((a&0xFF)<<24) | ((b&0xFF)<<16) | ((g&0xFF)<<8) | (r&0xFF);
	}

	static Color mix(const Color& col1, const Color& col2, uint8_t ratio, bool alpha1 = true)
	{
		// Mix col1 and col2 together, using 'ratio'
		// A ratio of 0 returns col1
		// A ratio of 255 returns col2 (with proper alpha processing according to alpha1)
		// A ratio of 127 retuns half of each color
		// If 'alpha1' is true, the alpha channel of 'col1' is used as-is in the returned color
		// If it is false, the alpha channel is also mixed
		if(ratio==0)
			return col1;

		if(ratio==255)
		{
			if(alpha1)
				return Color((((uint32_t)col2)&0xFFFFFF)|(col1.ia()<<24));
			else
				return col2;
		}

		uint32_t rr = 255-ratio;
		uint32_t ri = ratio;

		uint32_t r = ((col1.ir()*rr) + (col2.ir()*ri)) / 255;
		uint32_t g = ((col1.ig()*rr) + (col2.ig()*ri)) / 255;
		uint32_t b = ((col1.ib()*rr) + (col2.ib()*ri)) / 255;
		uint32_t a = alpha1 ? col1.ia() : ((col1.ia()*rr) + (col2.ia()*ri)) / 255;

		return Color(((a&0xFF)<<24) | ((b&0xFF)<<16) | ((g&0xFF)<<8) | (r&0xFF));
	}
};




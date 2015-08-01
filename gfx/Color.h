#pragma once


class Color
{
	uint32_t _val;
	// Actual color

	inline uint32_t f2i(float v)
	{
		// Clamp and convert a float component to an integer value in range 0..255
		return (uint32_t)(Math::fclamp(v)*255.0f);
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

	static Color mix(const Color& col1, const Color& col2, float ratio = 1.0f)
	{
		// Perform alpha blending
		float dr = col1.fr();
		float dg = col1.fg();
		float db = col1.fb();
		float da = col1.fa();

		float sr = col2.fr();
		float sg = col2.fg();
		float sb = col2.fb();
		float sa = ratio*col2.fa();

		float inv_sa = 1.0f - sa;

		float ra = sa + da*inv_sa;
		float rr = (sr*sa + dr*da*inv_sa)/ra;
		float rg = (sg*sa + dg*da*inv_sa)/ra;
		float rb = (sb*sa + db*da*inv_sa)/ra;


		return Color(rr, rg, rb, ra);
	}
};




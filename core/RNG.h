#pragma once


#define RNG_N 624

class RNG
{
private:
	uint32_t _seed;
	uint32_t _mt[RNG_N];
	int _mti;

public:

	RNG();
	RNG(uint32_t seed);
	RNG(const Str& seed);

	void set_seed(uint32_t seed);
	void set_seed(const Str& seed);
	// Set a new seed, resetting the RNG
	// The string version builds a 32-bit seed by performing simple math on the
	// ASCII value of the characters of the string

	uint32_t get_seed()
	{
		// Get the 32-bit seed
		return _seed;
	}

	uint32_t rnd32();
	// Generate a 32-bit random number

	int rnd31();
	// Generate a 31-bit random number

	float rndf();
	// Generate a floating point random numbe between 0 and 1

	int rnd(int min, int max);
	float rndf(float min, float max);
	double rndd(double min, double max);
	// Generate a random number between min and max, inclusive
};


extern RNG rng;
// A global RNG is always defined, to be used arbitrarily
// It is seeded using the system time



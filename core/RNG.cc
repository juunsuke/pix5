/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.						  

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

	 1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.

	 2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

	 3. The names of its contributors may not be used to endorse or promote 
		products derived from this software without specific prior written 
		permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)

   *** Modified to be integrated into PIX5
*/

#include "pix5core.h"
#include <time.h>


RNG rng;
// The global RNG


/* Period parameters */  
#define N RNG_N
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */


RNG::RNG()
{
	set_seed(time(0));
}

RNG::RNG(uint32_t seed)
{
	set_seed(seed);
}

RNG::RNG(const Str& seed)
{
	set_seed(seed);
}
	
void RNG::set_seed(uint32_t seed)
{
	_seed = seed;

	_mt[0] = seed & 0xffffffffUL;
	for (_mti=1; _mti<N; _mti++) {
		_mt[_mti] = (1812433253UL * (_mt[_mti-1] ^ (_mt[_mti-1] >> 30)) + _mti); 
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].						*/
		/* 2002/01/09 modified by Makoto Matsumoto			 */
		_mt[_mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}

void RNG::set_seed(const Str& seed)
{
	// Build a 32-bit number based on the given seed string
	uint32_t s = 1;

	for(const char *ch = seed; *ch; ch++)
		s = s*(*ch) + (*ch);

	set_seed(s);
}


/* generates a random number on [0,0xffffffff]-interval */
uint32_t RNG::rnd32()
{
	uint32_t y;
	static uint32_t mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (_mti == N) { /* generate N words at one time */
		int kk;

		for (kk=0;kk<N-M;kk++) {
			y = (_mt[kk]&UPPER_MASK)|(_mt[kk+1]&LOWER_MASK);
			_mt[kk] = _mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (_mt[kk]&UPPER_MASK)|(_mt[kk+1]&LOWER_MASK);
			_mt[kk] = _mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (_mt[N-1]&UPPER_MASK)|(_mt[0]&LOWER_MASK);
		_mt[N-1] = _mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		_mti = 0;
	}
  
	y = _mt[_mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

int RNG::rnd31()
{
	return (int)(rnd32()>>1);
}

float RNG::rndf()
{
	return (float)((double)rnd32() / (double)0xFFFFFFFF);
}

int RNG::rnd(int min, int max)
{
	return min + rnd31() % (max-min+1);
}

float RNG::rndf(float min, float max)
{
	return min + (max-min) * (float)((double)rnd32() * (1.0/4294967295.0));
}

double RNG::rndd(double min, double max)
{
	return min + (max-min) * (double)rnd32() * (1.0/4294967295.0);
}





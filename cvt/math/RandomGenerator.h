/*
   The MIT License (MIT)

   Copyright (c) 2011 - 2013, Philipp Heise and Sebastian Klose

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#ifndef CVT_RDFRANDOMGENERATOR_H
#define CVT_RDFRANDOMGENERATOR_H

#include <random>

namespace cvt {

/**
 * @brief Random Generator
 */
class RandomGenerator
{

   static std::mt19937& RAND_GENERATOR() { thread_local static std::mt19937 rand_generator; return rand_generator; }

  public:

   static void seed();

   static int   intRand( int min = 0, int max = 1 );

   static float floatRand( float min = 0.0f, float max = 1.0f );
   static float floatRandNormal( int m = 0, int s = 1 );

   static bool  boolRand( float p = 0.5f );

};

	/**
	 * Init random generator
	 */
	inline void RandomGenerator::seed()
	{
		auto const seed = std::random_device()();
		RAND_GENERATOR() = std::mt19937 ( seed );
	}

	/**
	 * Get random int between min and max
	 */
	inline int RandomGenerator::intRand( int min, int max )
	{
		std::uniform_int_distribution< int > distribution( min, max );
		return distribution( RAND_GENERATOR() );
	}

	/**
	 * Get random float between min and max
	 */
	inline float RandomGenerator::floatRand( float min, float max )
	{
		std::uniform_real_distribution< float > distribution( min, max );
		return distribution( RAND_GENERATOR() );
	}

	/**
	 * Get normal distributed random float
	 */
	inline float RandomGenerator::floatRandNormal( int m, int s )
	{
		std::normal_distribution< float > distribution( m, s );
		return distribution( RAND_GENERATOR() );
	}

	inline bool RandomGenerator::boolRand( float p )
	{
		std::bernoulli_distribution distribution( p );
		return distribution( RAND_GENERATOR() );
	}

}

#endif

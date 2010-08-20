#ifndef CVTMATH_H
#define CVTMATH_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <iostream>

namespace cvt {

<<<<<<< HEAD
	namespace Math {

=======
    namespace Math
	{
>>>>>>> Small fixes
		typedef union {
			uint32_t i;
			float f;
		} _flint32;

		typedef union {
			uint64_t i;
			double d;
		} _flint64;

		static const float  PI	     = 3.14159265358979323846f;
		static const float  TWO_PI   = 2.0f * PI;
		static const float  HALF_PI  = 0.5f * PI;
		static const float  E	     = 2.71828182845904523536f;
		static const float  EPSILONF = FLT_EPSILON;
		static const float  MAXF	 = FLT_MAX;
		static const double EPSILOND = DBL_EPSILON;
		static const double MAXD	 = DBL_MAX;

		template<class T> static inline T max( T x, T y ) { return ( x > y ) ? x : y; }
		template<class T> static inline T min( T x, T y ) { return ( x < y ) ? x : y; }
		template<class T> static inline size_t maxIndex( T x, T y ) { return  ( x > y ) ? 0 : 1; }
		template<class T> static inline size_t minIndex( T x, T y ) { return ( x < y ) ? 0 : 1; }

		template<typename T> static inline T mix( T a, T b, float alpha ) { return a + alpha * ( b - a ); }
		template<typename T> static inline T mix( T a, T b, double alpha ) { return a + alpha * ( b - a ); }

		template<typename T> static inline T clamp( T v, T min, T max ) { return ( v < min ) ? min : ( v > max ) ? max : v; }

		static inline size_t pad16( size_t v ) { return v & 0xf ?  ( ( v | 0xf ) + 1 ) : v; }

		template<typename T> static inline T sqr( T v ) { return v * v; }

		size_t solveQuadratic( float a, float b, float c, float (&result)[ 2 ] );
		size_t solveQuadratic( double a, double b, double c, double (&result)[ 2 ] );

		static inline float deg2Rad( float a )
		{
			return a * PI / 180.0f;
		}

		static inline float rad2Deg( float a )
		{
			return a * 180.0f / PI;
		}

		static inline float sqrt( float f )
		{
			return ::sqrtf( f );
		}

		static inline double sqrt( double d )
		{
			return ::sqrt( d );
		}

		static inline float invSqrt( float f )
		{
			return 1.0f / ::sqrtf( f );
		}

		static inline double invSqrt( double d )
		{
			return 1.0 / ::sqrt( d );
		}

		static inline float abs( float f )
		{
			_flint32 u;
			u.f = f;
			u.i &= 0x7FFFFFFF;
			return u.f;
		}

		static inline double abs( double d )
		{
			_flint64 u;
			u.d = d;
			u.i &= 0x7FFFFFFFFFFFFFFFULL;
			return u.d;
		}

		static inline float cos( float x )
		{
			return ::cosf( x );
		}

		static inline double cos( double x )
		{
			return ::cos( x );
		}

		static inline float sin( float x )
		{
			return ::sinf( x );
		}

		static inline double sin( double x )
		{
			return ::sin( x );
		}

		static inline float ceil( float x )
		{
			return ::ceilf( x );
		}

		static inline double ceil( double x )
		{
			return ::ceil( x );
		}

		static inline float floor( float x )
		{
			return ::floorf( x );
		}

		static inline double floor( double x )
		{
			return ::floor( x );
		}

		static inline float exp( float x )
		{
			return ::expf( x );
		}

		static inline double exp( double x )
		{
			return ::exp( x );
		}

		static inline float atan2( float y, float x )
		{
			return ::atan2f( y, x );
		}

		static inline double atan2( double y, double x )
		{
			return ::atan2( y, x );
		}

		static inline ssize_t rand()
		{
			return ( ssize_t ) ::random();
		}

		static inline float rand( float min, float max )
		{
			return min + ( ( max - min ) * ( ( float ) ::random() / ( ( float ) RAND_MAX + 1.0f )  ) );
		}

		static inline double rand( double min, double max )
		{
			return min + ( ( max - min ) * ( ( double ) ::random() / ( ( double ) RAND_MAX + 1.0 )  ) );
		}

		/*
		   The famous fast inverse square root approximation found in the code
		   of id-tech 3/4.
		 */
		static inline float fastInvSqrtf( float f )
		{
			_flint32 u;
			float fhalf;

			fhalf = 0.5f * f;
			u.f = f;
			u.i = 0x5F3759DF - ( u.i >> 1 );
			f = u.f;
			return f * ( 1.5f - fhalf * f * f );
		}

		/* Round to nearest integer */
		static inline int rint( float f ) {
			int i;
			__asm__ __volatile__ ("fistpl %0" : "=m" (i) : "t" (f) : "st") ;
			return i;
		}

	}
}


#endif

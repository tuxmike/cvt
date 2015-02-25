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

#ifndef CVT_RDFDATAPOINT_H
#define CVT_RDFDATAPOINT_H

#include <vector>
#include <istream>
#include <ostream>
#include <sstream>

namespace cvt {

	template < typename I, typename O >  class RDFDataPoint;
	template < typename I, typename O >  std::ostream& operator<< ( std::ostream& os, const RDFDataPoint<I,O>& s );

	/**
	 * Represents a Data Input-Output tuple
	 * I: input datatype, O: output datatype
	 */
	template< typename I, typename O >
	class RDFDataPoint
	{
	  public:

		typedef I InputType;
		typedef O OutputType;


		RDFDataPoint( const I& input, const O& output );
		RDFDataPoint( const RDFDataPoint& other );

		~RDFDataPoint();

		RDFDataPoint& operator=( const RDFDataPoint& other );
		bool       operator==( const RDFDataPoint& other );
		friend std::ostream& operator<< <>( std::ostream& os, const RDFDataPoint& point );

		const I& input() const;
		const O& output() const;

	  private:
		I   myinput;
		O   myoutput;
	};



	/**
	 * DataPoint with input and output data
	 */
	template< typename I, typename O >
	inline RDFDataPoint<I,O>::RDFDataPoint( const I& input, const O& output ) :
	  myinput( input ),
	  myoutput( output )
	{}

	/**
	 * DataPoint from other DataPoint
	 */
	template< typename I, typename O >
	inline RDFDataPoint<I,O>::RDFDataPoint( const RDFDataPoint& other ) :
	  myinput( other.myinput ),
	  myoutput( other.myoutput )
	{}

	template< typename I, typename O >
	inline RDFDataPoint<I,O>::~RDFDataPoint()
	{}

	/**
	 * Copy DataPoint
	 */
	template< typename I, typename O >
	inline RDFDataPoint<I,O>& RDFDataPoint<I,O>::operator=( const RDFDataPoint& other )
	{
	  if( this != &other )
	  {
		myinput = other.myinput;
		myoutput = other.myoutput;
	  }
	  return *this;
	}

	/**
	 * Compare
	 */
	template< typename I, typename O >
	inline bool RDFDataPoint<I,O>::operator==( const RDFDataPoint& other )
	{
	  return myinput == other.input && myoutput == other.output;
	}

	/**
	 * Output DataPoint
	 */
	template< typename I, typename O >
	inline std::ostream& operator<<( std::ostream& os, const RDFDataPoint<I,O>& point )
	{
	  os << "(";
	  for (int i = 0; i < point.input_.size() - 1; i++ )
	  {
		os << point.input_[ i ] << ",";
	  }
	  os << point.input_.back() << "),";

	  os << point.output_;

	  return os;
	}

	/**
	 * Get input
	 */
	template< typename I, typename O >
	inline const I& RDFDataPoint<I,O>::input() const
	{
	  return myinput;
	}

	/**
	 * Get output
	 */
	template< typename I, typename O >
	inline const O& RDFDataPoint<I,O>::output() const
	{
	  return myoutput;
	}

}

#endif

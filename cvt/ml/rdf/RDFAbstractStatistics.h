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

#ifndef CVT_RDFABSTRACTSTATISTICS_H
#define CVT_RDFABSTRACTSTATISTICS_H

#include <vector>
#include <io/xml/XMLSerializable.h>

namespace cvt {

	/**
	 * @brief Abstract Statistics
	 */
	template< typename DerivedType, typename DataType >
	class RDFAbstractStatistics : public cvt::XMLSerializable
	{
	  public:

		typedef typename DataType::OutputType OutputType;

		/**
		 * Combine statistics
		 */
		virtual DerivedType& operator+=( const DerivedType& s ) =0;

		/**
		 * Increment specific output class
		 */
		virtual DerivedType& operator+=( const OutputType& o ) =0;

		/**
		 * Return max class and according value
		 */
		virtual std::pair< OutputType, float > predict() const =0;

		/**
		 * @brief Compute information gain referring to parent
		 *
		 * @return float
		 */
		virtual float computeInformationGain( DerivedType& leftStatistics,
				DerivedType& rightStatistics ) = 0;
	};

}

#endif


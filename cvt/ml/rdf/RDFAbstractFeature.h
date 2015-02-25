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

#ifndef CVT_RDFABSTRACTFEATURE_H
#define CVT_RDFABSTRACTFEATURE_H

#include <vector>
#include <io/xml/XMLSerializable.h>
#include <math/RandomGenerator.h>

namespace cvt {

	/**
	 * @brief Abstract Feature
	 */
	template< typename DataT >
	class RDFAbstractFeature : public cvt::XMLSerializable
	{

	  public:

		class FeatureSampler
		{

		  public:

			/**
			 * @brief Create Features
			 * @param number of Features
			 * @return features
			 */
			virtual void sample( std::vector< RDFAbstractFeature >& features, size_t numFeatures ) =0;

			virtual ~FeatureSampler() {};

		};

		typedef DataT DataType;

		/**
		 * @brief Feature Test Function
		 *
		 * @param input vectors
		 * @return bool
		 */
		virtual bool operator()( const typename DataType::InputType& input ) const =0;
	};

}

#endif

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

#ifndef CVT_RDFABSTRACTRDFCONTEXT_H
#define CVT_RDFABSTRACTRDFCONTEXT_H

#include <vector>
#include <io/xml/XMLSerializable.h>
#include <math/RandomGenerator.h>
#include <ml/rdf/RDFAbstractFeature.h>
#include <ml/rdf/RDFAbstractStatistics.h>
#include <ml/rdf/RDFDataPoint.h>

namespace cvt {

	/**
	 * Define Types for a RDF Setup
	 */
	class RDFAbstractContext
	{
	   public:

		/*
		 * Shadow in individual Context
		 */
		typedef std::vector< float >                  InputType;
		typedef size_t                                OutputType;
		typedef RDFDataPoint < InputType, OutputType >   DataType;

		typedef RDFAbstractFeature< DataType >       	  FeatureType;
		typedef FeatureType::FeatureSampler 		  FeatureSamplerType;
		//typedef RDFAbstractStatistics< DataType >      StatisticsType;

	   private:
		// no instances
		RDFAbstractContext();
	};

}

#endif

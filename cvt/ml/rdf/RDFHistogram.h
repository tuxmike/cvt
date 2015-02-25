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

#ifndef CVT_RDFHISTOGRAM_H
#define CVT_RDFHISTOGRAM_H

#include <io/xml/XMLElement.h>
#include <io/xml/XMLAttribute.h>
#include <ml/rdf/RDFAbstractStatistics.h>


namespace cvt {

	template < typename DataType >	class RDFHistogram;
	template < typename DataType >  std::ostream& operator<< ( std::ostream& os, const RDFHistogram<DataType>& s );

	/**
	 * Histogram-Statistic for RDF leaf-nodes
	 * OutputType
	 */
	template< typename DataType >
	class RDFHistogram: public RDFAbstractStatistics< RDFHistogram< DataType>, DataType >
	{

	  public:
		typedef typename DataType::OutputType OutputType;

		RDFHistogram();
		RDFHistogram( const RDFHistogram& other );

		~RDFHistogram();

		RDFHistogram& operator=( const RDFHistogram& other );
		RDFHistogram& operator+=( const OutputType& output );
		RDFHistogram& operator+=( const RDFHistogram& h );

		std::pair< OutputType, float > predict() const;

		float computeInformationGain( RDFHistogram& leftStatistics, RDFHistogram& rightStatistics );
		float getEntropy();
		float probability( size_t classIndex ) const;

		size_t n() const;
		size_t getNumClasses() const;

		friend std::ostream& operator<< <>( std::ostream& os, const RDFHistogram<DataType>& s );

		cvt::XMLNode* serialize() const;
		void 		  deserialize( cvt::XMLNode* node );

	  private:
		static size_t 		   NUM_CLASSES;

		size_t                 num;
		std::vector< size_t >  histogram;
		float                  entropy;

	};

}

#endif

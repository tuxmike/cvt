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

#include <math/Math.h>
#include <io/xml/XMLElement.h>
#include <io/xml/XMLAttribute.h>
#include <ml/rdf/RDFAbstractStatistics.h>
#include <ml/rdf/RDFHistogram.h>

namespace cvt {

	template< typename DataType >
	size_t RDFHistogram<DataType>::NUM_CLASSES = 0;

	/**
	 * Init histogram with zeros
	 */
	template< typename DataType >
	RDFHistogram<DataType>::RDFHistogram( ) :
	  num( 0 ),
	  entropy( -1.0f ),
	  histogram ( NUM_CLASSES , 0 )
	{}

	/**
	 * Copy C'tor
	 */
	template< typename DataType >
	RDFHistogram<DataType>::RDFHistogram( const RDFHistogram& other ) :
	  histogram( other.histogram ),
	  num( other.num ),
	  entropy( other.entropy )
	{}

	template< typename DataType >
	RDFHistogram<DataType>::~RDFHistogram()
	{}


	/**
	 * Copy
	 */
	template< typename DataType >
	RDFHistogram<DataType>& RDFHistogram<DataType>::operator=( const RDFHistogram& other )
	{
	  if( this != &other )
	  {
		num = other.num;
		histogram = other.histogram;
		entropy = other.entropy;
	  }
	  return *this;
	}

	/**
	 * Increment class count for output
	 */
	template< typename DataType >
	RDFHistogram<DataType>& RDFHistogram<DataType>::operator+=( const OutputType& output )
	{
	  histogram[ output ]++;
	  num++;
	  entropy = -1.0f;
	  return *this;
	}

	/**
	 * Sum up histograms
	 */
	template< typename DataType >
	RDFHistogram<DataType>& RDFHistogram<DataType>::operator+=( const RDFHistogram& h )
	{
	  typename std::vector< size_t >::const_iterator sit = h.histogram.begin(),
		send = h.histogram.end();
	  typename std::vector< size_t >::iterator it = histogram.begin();

	  for( ; sit != send; ++sit, ++it )
	  {
		( *it ) += ( *sit );
	  }
	  num += h.num;
	  entropy = -1.0f;
	  return *this;
	}

	/**
	 * @brief Returns mode of histogram with the corresponding
	 * empirical class probability
	 *
	 * @return pair(maxClassIdx, maxValue)
	 */
	template< typename DataType >
	std::pair< typename DataType::OutputType, float > RDFHistogram<DataType>::predict() const
	{
	  float maxValue = FLT_MIN;
	  size_t maxC;

	  for( size_t i = 0; i < histogram.size(); i++ )
	  {
		if( histogram[ i ] > maxValue )
		{
		  maxValue = histogram[ i ];
		  maxC = i;
		}
	  }

	  return std::pair< OutputType, float >( maxC, maxValue / num );
	}

	/**
	 * @brief Compute information gain referring to parent
	 *
	 * @return float
	 */
	template< typename DataType >
	float RDFHistogram<DataType>::computeInformationGain( RDFHistogram& leftStatistics,
			RDFHistogram& rightStatistics )
	{
		float Hp = getEntropy();

		float Hl = leftStatistics.getEntropy();
		float Hr = rightStatistics.getEntropy();

		float fraction = leftStatistics.n() / static_cast<float>( n() )			;

		return Hp - ( ( fraction * Hl ) + ( ( 1.0f  - fraction ) * Hr ) );
	}

	/**
	 * @brief Calculates entropy of the histogram
	 *
	 * @return
	 */
	template< typename DataType >
	float RDFHistogram<DataType>::getEntropy()
	{
	  if( entropy < 0 )
	  {
		entropy = 0.0f;
		if( num > 0 )
		{
		  typename std::vector< size_t >::const_iterator it = histogram.begin(),
				   end = histogram.end();

		  for( ; it != end; ++it )
		  {
			if( float p_i = static_cast<float>( *it ) / num )
			{
			  entropy -= p_i * cvt::Math::log2( p_i );
			}
		  }
		}
	  }
	  return entropy;
	}

	/**
	 * @brief Calculates probability for a classIndex
	 *
	 * @return
	 */
	template< typename DataType >
	float RDFHistogram<DataType>::probability( size_t classIndex ) const
	{
	  if( !histogram[ classIndex ] )
	  {
		return 0.0f;
	  } else {
		return static_cast<float>( histogram[ classIndex ] ) / num;
	  }
	}

	/**
	 * Return number of elements counted
	 */
	template< typename DataType >
	size_t RDFHistogram<DataType>::n() const
	{
	  return num;
	}

	/**
	 * Return number of classes
	 */
	template< typename DataType >
	size_t RDFHistogram<DataType>::getNumClasses() const
	{
		return histogram.size();
	}

	/**
	 * Output classes and value
	 */
	template< typename DataType >
	std::ostream& operator<<( std::ostream& os, const RDFHistogram<DataType>& s )
	{
	  os << s.num << ": { ";
	  typename std::vector< size_t >::const_iterator it = s.histogram.begin(),
		end = s.histogram.end();
	  for( size_t c = 0; it != end; ++it, c++ )
	  {
		os << "(" << c << "," << *it << ") ";
	  }
	  os << "}";

	  return os;
	}

	template< typename DataType >
	cvt::XMLNode* RDFHistogram<DataType>::serialize() const
	{
	  cvt::XMLElement* node = new cvt::XMLElement( "Histogram");
	  cvt::String s;
	  cvt::XMLAttribute* attr;
	  cvt::XMLElement* elem;

	  s.sprintf( "%d", num );
	  attr = new cvt::XMLAttribute( "n", s );
	  node->addChild( attr );

	  s.sprintf( "%f", entropy );
	  attr = new cvt::XMLAttribute( "entropy", s );
	  node->addChild( attr );

	  elem = new cvt::XMLElement( "histogram" );
	  s.sprintf( "%d", histogram.size() );
	  attr = new cvt::XMLAttribute( "size", s );
	  elem->addChild( attr );
	  cvt::XMLElement* point;
	  for( size_t i = 0; i < histogram.size(); i++ )
	  {
		point = new cvt::XMLElement( "class" );
		s.sprintf( "%d", i );
		attr = new cvt::XMLAttribute( "index", s );
		point->addChild( attr );
		s.sprintf( "%d", histogram[ i ] );
		attr = new cvt::XMLAttribute( "count", s );
		point->addChild( attr );
		elem->addChild( point );
	  }
	  node->addChild( elem );

	  return node;
	}

	template< typename DataType >
	void RDFHistogram<DataType>::deserialize( cvt::XMLNode* node )
	{
	  num = node->childByName( "n" )->value().toInteger();

	  entropy = node->childByName( "entropy" )->value().toFloat();

	  cvt::XMLNode* n = node->childByName( "histogram" );
	  histogram.resize( n->childByName( "size" )->value().toInteger(), 0);
	  for( size_t i = 0; i < n->childSize(); i++ )
	  {
		cvt::XMLNode* entry = n->child( i );
		if( entry->name() != "class" )
		{
		  continue;
		}
		size_t idx = entry->childByName( "index" )->value().toInteger();
		histogram[ idx ] = entry->childByName( "count" )->value().toInteger();
	  }
	}

}

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

#ifndef CVT_RDFFOREST_H
#define CVT_RDFFOREST_H

#include <vector>
#include <io/xml/XMLSerializable.h>
#include <io/xml/XMLElement.h>
#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFTree.h>
#include <ml/rdf/RDFTreeTrainer.h>
#include <ml/rdf/RDFTrainingParameters.h>

#define CVT_RDF_THREAD_NUM_AUTO 0

namespace cvt {

	template< typename RDFContext > class RDFTreeTrainer;

	/**
	 * Random Decision Forest
	 */
	template< typename RDFContext, typename TreeTrainerType = RDFTreeTrainer<RDFContext> >
	class RDFForest : public XMLSerializable
	{
		typedef RDFTree< RDFContext >                    TreeType;
		typedef typename RDFContext::StatisticsType      StatisticsType;
		typedef typename RDFContext::DataType            DataType;
		typedef typename RDFContext::InputType           InputType;
		typedef typename RDFContext::FeatureSamplerType  FeatureSamplerType;


	  public:

		RDFForest();
		virtual ~RDFForest();

		void add( const TreeType& tree );
		void evaluate( std::vector< const StatisticsType* >& statistics, const InputType& input ) const;
		void evaluate( StatisticsType& s, const InputType& input ) const;

		void enqueueTreeTraining( const std::vector< DataType >& data, const RDFTrainingParameters& params, const FeatureSamplerType& sampler, size_t numTrees = 0 );
		void train( unsigned threads = 1, bool showProgress = false );

		XMLNode* serialize() const;
		void     deserialize( XMLNode* node );

	  private:

		void trainMultithread( bool showProgress = false, unsigned threads = 0 );
		void trainSinglethread( bool showProgress = false );

		std::vector< TreeType >  trees;
		std::vector< TreeTrainerType > trainers;
	};

}

#endif

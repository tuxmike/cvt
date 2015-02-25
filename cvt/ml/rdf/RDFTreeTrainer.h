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

#ifndef CVT_RDFTREETRAINER_H
#define CVT_RDFTREETRAINER_H

#include <math/Math.h>
#include <math/RandomGenerator.h>
#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFForest.h>
#include <ml/rdf/RDFPath.h>
#include <ml/rdf/RDFTrainingParameters.h>
#include <vector>

namespace cvt {

	/**
	 * @brief Train RDF-Tree
	 */
	template< typename RDFContext >
	class RDFTreeTrainer
	{
	  public:
			typedef RDFTree< RDFContext >                   TreeType;
			typedef typename TreeType::Node               	NodeType;

			typedef typename RDFContext::InputType        	InputType;
			typedef typename RDFContext::OutputType       	OutputType;
			typedef typename RDFContext::DataType         	DataType;

			typedef typename RDFContext::FeatureType      	FeatureType;
			typedef typename RDFContext::FeatureSamplerType FeatureSamplerType;
			typedef typename RDFContext::StatisticsType		StatisticsType;


			RDFTreeTrainer( const std::vector< DataType >& data, const RDFTrainingParameters& params, const FeatureSamplerType sampler );
			RDFTreeTrainer( const RDFTreeTrainer& other );

			virtual ~RDFTreeTrainer();

			virtual void train( TreeType& tree, bool showProgress );

	  protected:

			virtual bool 			shouldTerminate( float informationGain ) const;
			virtual StatisticsType  getRootStatistics( ) const;
			virtual void 			allocateCandidateStatistics( size_t numFeatures );
			virtual void 			fillCandidateStatistics();

			virtual bool 			selectBestSplits();
			virtual bool 			writeSplit( NodeType* n, float gain, size_t testIdx, const std::vector< StatisticsType >& statistics, const RDFPath& path );

			virtual void 			updateDataPaths();

			static inline void 		loadbar(unsigned int x, unsigned int n, unsigned int w = 50);


			const std::vector< DataType >* data_;
			const RDFTrainingParameters params;
			FeatureSamplerType sampler;

			TreeType* tree_;
			size_t depth = 0;
			size_t numNodesD = 0;
			std::vector< RDFPath > paths;
			std::vector< RDFPath > blacklist;
			std::vector< FeatureType > randomFeatures;
			std::vector< std::vector< StatisticsType > > candidateStatistics;
	};

}

#endif

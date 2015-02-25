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

#include <ml/rdf/RDFTreeTrainer.h>
#include <math/Math.h>
#include <math/RandomGenerator.h>
#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFForest.h>
#include <ml/rdf/RDFPath.h>
#include <ml/rdf/RDFTrainingParameters.h>
#include <ml/rdf/RDFTree.h>
#include <vector>

namespace cvt {

	template< typename RDFContext >
	RDFTreeTrainer<RDFContext>::RDFTreeTrainer( const std::vector< DataType >& data, const RDFTrainingParameters& params, const FeatureSamplerType sampler )
		: data_ ( &data ), params ( params ), sampler ( sampler ), tree_ ( NULL ) {}

	/**
	 * Copy C'tor
	 */
	template< typename RDFContext >
	RDFTreeTrainer<RDFContext>::RDFTreeTrainer( const RDFTreeTrainer& other ) : data_ ( other.data_ ), params ( other.params ), sampler ( other.sampler ), tree_ ( NULL ) {}

	template< typename RDFContext >
	RDFTreeTrainer<RDFContext>::~RDFTreeTrainer() {}

	/**
	 * @brief Criterion if a leaf should not be further split
	 * @param informationGain
	 * @return
	 */
	template< typename RDFContext >
	bool RDFTreeTrainer<RDFContext>::shouldTerminate( float informationGain ) const
	{
		// TODO Magic number
		return informationGain < 0.01f;
	}

	/**
	 * Fill root statistics with all data
	 */
	template< typename RDFContext >
	typename RDFTreeTrainer<RDFContext>::StatisticsType RDFTreeTrainer<RDFContext>::getRootStatistics( ) const
	{
		StatisticsType s;
		for( size_t i = 0; i < data_->size(); ++i )
		{
		  s += (*data_)[ i ].output();
		}
		return s;
	};

	/**
	 * Allocate "2 * numFeatures" Statistics per Node
	 */
	template< typename RDFContext >
	void RDFTreeTrainer<RDFContext>::allocateCandidateStatistics( size_t numFeatures )
	{
		for(size_t i = 0; i < numNodesD; i++ )
		{
			const RDFPath path( i, depth );
			if( !path.isBlacklisted( blacklist ) )
			{
				candidateStatistics[ i ] =  std::vector< StatisticsType > ( numFeatures * 2, StatisticsType() );
			}
		}
	}

	/**
	 * Fill data into candidate statistics
	 * using sampled features
	 */
	template< typename RDFContext >
	void RDFTreeTrainer<RDFContext>::fillCandidateStatistics()
	{
		for( size_t i = 0; i < data_->size(); i++ )
		{
			const RDFPath& path = paths[ i ];
			if( path.isBlacklisted( blacklist ) )
			{
				continue;
			}

			std::vector< StatisticsType >& statistics = candidateStatistics[ path.path() ];
			for( size_t t = 0; t < randomFeatures.size(); t++ )
			{

				bool result = randomFeatures[ t ]( (*data_)[ i ].input() );

				size_t statisticIdx = 2 * t + result; // result 0: left child, 1: right child

				statistics[ statisticIdx ] += ( (*data_)[ i ].output() ) ;
			}
		}
	}

	/**
	 * Fore each node, choose beest split
	 * from candidate statistics
	 * and write it to tree
	 *
	 * @return bool: tree growing
	 */
	template< typename RDFContext >
	bool RDFTreeTrainer<RDFContext>::selectBestSplits()
	{
		bool growing = false;
		for( size_t i = 0; i < numNodesD; i++ )
		{
			const RDFPath path( i, depth );
			if( path.isBlacklisted( blacklist ) )
			{
				continue;
			}

			NodeType* n = tree_->getNode( path );
			std::vector< StatisticsType >& statistics = candidateStatistics[ i ];

			// select test by gain
			size_t bestTestIdx = 0;
			float bestGain = -FLT_MAX;

			for( size_t t = 0; t < randomFeatures.size(); t++ )
			{
				float gain = n->statistics.computeInformationGain( statistics[ 2 * t ], statistics[ 2 * t + 1 ] );
				if( gain > bestGain )
				{
					bestTestIdx = t;
					bestGain = gain;
				}
			}

			// write split
			bool split = writeSplit( n, bestGain, bestTestIdx, statistics, path );
			growing = growing || split;
		}

		return growing;
	}

	/**
	 * If split is good enough, write it to tree
	 * @return bool: wrote split
	 */
	template< typename RDFContext >
	bool RDFTreeTrainer<RDFContext>::writeSplit( NodeType* n, float gain, size_t testIdx, const std::vector< StatisticsType >& statistics, const RDFPath& path )
	{
		// check for good similarity -> no further splitting
		if( shouldTerminate( gain ) && path.depth() > 1 )
		{
		  blacklist.push_back( path );
		  return false;
		}
		else
		{
			tree_->hydrateSplitNode(
					n,
					randomFeatures[ testIdx ],
					statistics[ 2 * testIdx ],
					statistics[ 2 * testIdx + 1 ]
					);
			return true;
		}
	}

	/**
	 * Update paths data has taken
	 */
	template< typename RDFContext >
	void RDFTreeTrainer<RDFContext>::updateDataPaths()
	{
		for( size_t i = 0; i < data_->size(); i++ )
		{
			RDFPath& path = paths[ i ];
			const NodeType* n = tree_->getNode( path );
			if( !path.isBlacklisted( blacklist ) && n->isSplit() )
			{
				path.add( n->feature( (*data_)[ i ].input() ) );
			}
		}
	}

	/**
	 * Show training progress
	 */
	template< typename RDFContext >
	void RDFTreeTrainer<RDFContext>::loadbar( unsigned int x, unsigned int n, unsigned int w )
	{
	  if ( (x != n) && (x % (n/100+1) != 0) ) return;

	  float ratio  =  x/(float)n;
	  int   c      =  ratio * w;

	  std::cout << std::setw(3) << (int)(ratio*100) << "% [";
	  for (int x=0; x<c; x++) std::cout << "=";
	  for (int x=c; x<w; x++) std::cout << " ";
	  std::cout << "]\r" << std::flush;
	}

	/**
	 * Creates tree in breadth-first order.
	 *
	 * @return
	 */
	template< typename RDFContext >
	void RDFTreeTrainer<RDFContext>::train( TreeType& tree, bool showProgress )
	{
		tree_ = &tree;

		tree_->initTree( params.maxDepth, getRootStatistics() );

		RandomGenerator::seed();

		paths = std::vector< RDFPath > ( data_->size(), RDFPath( 0, 0 ) );
		blacklist = std::vector< RDFPath > ();

		// At every tree level expand all frontier nodes
		for( depth = 0; depth < params.maxDepth; depth++ )
		{
			if( showProgress )
			{
			  loadbar( depth, params.maxDepth );
			}

			numNodesD = 1UL << depth;

			// sample tests
			randomFeatures.clear();
			sampler.sample( randomFeatures, params.tests );

			// allocate statistics
			candidateStatistics.clear();
			candidateStatistics.resize( numNodesD );
			allocateCandidateStatistics( randomFeatures.size() );

			// fill statistics
			fillCandidateStatistics();

			// select splits and write them to tree
			bool growing = selectBestSplits();

			randomFeatures.clear();
			candidateStatistics.clear();

			if( !growing )
			{
			  std::cout << "tree not growing, exit" << std::endl;
			  break;
			}

			updateDataPaths();

		}

		// free
		std::vector< FeatureType >().swap( randomFeatures );
		std::vector< std::vector< StatisticsType > >().swap( candidateStatistics );
		std::vector< RDFPath >().swap( blacklist );
		std::vector< RDFPath >().swap( paths );

		if( showProgress )
		{
			loadbar( 1, 1 );
			std::cout << std::endl;
		}

	}
}

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

#include <vector>
#include <thread>
#include <algorithm>
#include <io/xml/XMLSerializable.h>
#include <io/xml/XMLElement.h>
#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFForest.h>
#include <ml/rdf/RDFTree.h>
#include <ml/rdf/RDFTreeTrainer.h>

#define CVT_RDF_THREAD_NUM_AUTO 0

namespace cvt {

		/**
		 * Add a tree
		 */
		template< typename RDFContext, typename TreeTrainerType>
		void RDFForest<RDFContext,TreeTrainerType>::add( const TreeType& tree )
		{
		  trees.push_back( tree );
		}

		/**
		 * Traverse Forest for specific input
		 * return statistics from every tree
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::evaluate( std::vector< const StatisticsType* >& statistics, const InputType& input ) const
		{
		  statistics.reserve( trees.size() );
		  typename std::vector< TreeType >::const_iterator it = trees.begin(),
			end = trees.end();
		  for( ; it != end; ++it )
		  {
			statistics.push_back( &( it->evaluate( input ) ) );
		  }
		}

		/**
		 * Traverse Forest for specific input
		 * return aggregated statistics
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::evaluate( StatisticsType& s, const InputType& input ) const
		{
		  typename std::vector< TreeType >::const_iterator it = trees.begin(),
			end = trees.end();
		  for( ; it != end; ++it )
		  {
			s += it->evaluate( input );
		  }
		}

		/**
		 * Enqueue Data for Training one or more trees
		 * numTrees = 0 : train number of trees from params (default)
		 * numTrees > 0 : override number of trees from params
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::enqueueTreeTraining( const std::vector< DataType >& data,
								  const RDFTrainingParameters& params,
								  const FeatureSamplerType& sampler,
								  size_t numTrees )
		{
			if( !numTrees ) numTrees = params.trees;

			for( size_t t = 0; t < numTrees; t++ ) {
				trainers.emplace_back( data, params, sampler );
			}
		}

		/**
		 * Run Tree Training
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::train( unsigned threads, bool showProgress )
		{
			if( threads != 1 ) {
				trainMultithread( showProgress, threads );
			} else {
				trainSinglethread( showProgress );
			}
		}

		template< typename RDFContext, typename TreeTrainerType >
		XMLNode* RDFForest<RDFContext,TreeTrainerType>::serialize() const
		{
		  XMLElement* node = new XMLElement( "Forest" );

		  for( size_t i = 0; i < trees.size(); i++ )
		  {
			node->addChild( trees[ i ].serialize() );
		  }

		  return node;
		}

		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::deserialize( XMLNode* node )
		{
		  for( size_t i = 0; i < node->childSize(); i++ )
		  {
			TreeType tree;
			tree.deserialize( node->child( i ) );
			trees.push_back( tree );
		  }
		}



		/**
		 * @brief Create trees and train with simple multithreading
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::trainMultithread( bool showProgress, unsigned threads )
		{
		  /*
		   * Simple parallel training:
		   *   +----tree1----+    +----tree6----+
		   *   +----tree2----+    +----tree7----+
		   * +-+----tree3----+----+----tree8----+---
		   *   +----tree4----+    +----tree9----+
		   *   +----tree5----+    +----tree10---+
		   */

		  unsigned maxThreads = std::thread::hardware_concurrency();
		  unsigned numThreads = ( threads == CVT_RDF_THREAD_NUM_AUTO || threads >  maxThreads ) ? maxThreads : threads;

		  unsigned numNewTrees = trainers.size();
		  unsigned treeCount = 0;

		  trees.reserve( trees.size() + numNewTrees );

		  while( treeCount < numNewTrees )
		  {
				std::vector< std::thread > threads;
				std::vector< TreeType > trainingTrees;

				threads.reserve( numThreads );
				trainingTrees.reserve( numThreads );

				for( unsigned t = 0; t < numThreads; t++, treeCount++ ) {

					if( treeCount >= numNewTrees ) {
					   break;
					}

					if( showProgress )
					{
					   std::cout << "Spawning Training-Thread for tree " << treeCount + 1 << std::endl;
					}

					trainingTrees.emplace_back();

					threads.push_back( std::thread( &TreeTrainerType::train, trainers[ treeCount ], std::ref( trainingTrees.back() ), false ) );

				}

				for( unsigned i = 0; i < threads.size(); i++ ) {
					threads[ i ].join();
					add( trainingTrees[ i ] );
				}
		  }

		  trainers.clear();

		}

		/**
		 * @brief Create trees and train without multithreading
		 */
		template< typename RDFContext, typename TreeTrainerType >
		void RDFForest<RDFContext,TreeTrainerType>::trainSinglethread( bool showProgress )
		{
			unsigned numNewTrees = trainers.size();
			trees.reserve( trees.size() + numNewTrees );

			for( size_t treeCount = 0; treeCount < numNewTrees; treeCount++ )
			{
				if( showProgress )
				{
				  std::cout << "Training tree " << treeCount + 1 << "/" << numNewTrees << std::endl;
				}

			TreeType tree;
			trainers[ treeCount ].train( tree, showProgress );
			add( tree );

			}

			trainers.clear();
		}

}

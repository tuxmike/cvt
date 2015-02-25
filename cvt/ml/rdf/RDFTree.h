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

#ifndef CVT_RDFTREE_H
#define CVT_RDFTREE_H

#include <iomanip>
#include <vector>
#include <memory>

#include <io/xml/XMLElement.h>
#include <io/xml/XMLAttribute.h>

#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFPath.h>

namespace cvt {

	template < typename RDFContext >  class RDFTree;
	template < typename RDFContext >  std::ostream& operator<< ( std::ostream& os, const RDFTree<RDFContext>& s );

	/**
	 * Random Decision Tree
	 */
	template< typename RDFContext >
	class RDFTree
	{

	  public:

		typedef typename RDFContext::FeatureType      FeatureType;
		typedef typename RDFContext::StatisticsType  StatisticsType;
		typedef typename RDFContext::InputType       InputType;

		/**
		 * Random Decision Tree Node
		 */
		class Node
		{
		  public:

			Node() : split( false ) {};

			bool isSplit() const {	return split; }

			FeatureType feature;
			StatisticsType statistics;
			bool split;

		};

		RDFTree();
		virtual ~RDFTree();

		void initTree( size_t maxdepth, const StatisticsType& rootStatistics );

		Node* leftChild(const Node* node) const;
		Node* rightChild(const Node* node) const;
		Node* getNode(const RDFPath path);

		void hydrateSplitNode( Node* node, const FeatureType& feature, const StatisticsType& leftS, const StatisticsType& rightS );

		StatisticsType& evaluate( const InputType& input ) const;

		XMLNode* serialize() const;
		void     deserialize( XMLNode* node );

		friend std::ostream& operator<< <>( std::ostream& os, const RDFTree& tree );
		void preorder( std::ostream& os, Node* p, int indent=0) const;


	  private:

		XMLNode* serializeNode(const Node* node_) const;
		void     deserializeNode( Node* node_, XMLNode* nodeSer_ );

		Node* root_;
		Node* nodes_;
		size_t maxDepth;
		size_t maxElements;

		std::shared_ptr< Node > nodes;

	};

}

#endif

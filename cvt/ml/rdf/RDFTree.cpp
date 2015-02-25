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

#include <iomanip>
#include <vector>
#include <memory>

#include <io/xml/XMLElement.h>
#include <io/xml/XMLAttribute.h>

#include <ml/rdf/RDFAbstractContext.h>
#include <ml/rdf/RDFPath.h>
#include <ml/rdf/RDFTree.h>

namespace cvt {


	template< typename RDFContext >
	RDFTree<RDFContext>::RDFTree() : maxDepth( 0 ),	maxElements ( 0 ), root_ ( NULL ), nodes_ ( NULL ) {}


	template< typename RDFContext >
	RDFTree<RDFContext>::~RDFTree()
	{
		//delete[] nodes_;
	}

	/**
	 * Init Tree with max depth and root
	 */
	template< typename RDFContext >
	void RDFTree<RDFContext>::initTree( size_t maxdepth, const StatisticsType& rootStatistics )
	{
		// Reserve max needed memory
		maxDepth = maxdepth;
		maxElements = 1UL << ( maxDepth + 1 );
		nodes = std::shared_ptr< Node > (
				new Node[ maxElements ], std::default_delete< Node[] >() );
		nodes_ = nodes.get();
		root_ = &nodes_[0];
		root_->statistics = rootStatistics;
	}

	/**
	 * Left Child
	 */
	template< typename RDFContext >
	typename RDFTree<RDFContext>::Node* RDFTree<RDFContext>::leftChild(const Node* node) const
	{
		const ptrdiff_t idx = node - nodes_;
		return &nodes_[ idx * 2 + 1 ];
	}

	/**
	 * Right Child
	 */
	template< typename RDFContext >
	typename RDFTree<RDFContext>::Node* RDFTree<RDFContext>::rightChild(const Node* node) const
	{
		const ptrdiff_t idx = node - nodes_;
		return &nodes_[ idx * 2 + 2 ];
	}


	/**
	 * Get Node by depth and row-index in depth
	 */
	template< typename RDFContext >
	typename RDFTree<RDFContext>::Node* RDFTree<RDFContext>::getNode(const RDFPath path)
	{
		if( path.depth() == 0 )
		{
			return root_;
		}
		else
		{
			/* if( path.index() >= maxElements )
			{
				std::cout <<  "Error:" << path.index() << " / " <<  maxElements << std::endl;
			}
			*/
			return &nodes_[ path.index() ];
		}
	}

	/**
	 * Set left and right statistics and node feature
	 */
	template< typename RDFContext >
	void RDFTree<RDFContext>::hydrateSplitNode( Node* node, const FeatureType& feature, const StatisticsType& leftS, const StatisticsType& rightS )
	{
	  node->feature = feature;
	  node->split = true;
	  leftChild( node )->statistics = leftS;
	  rightChild( node )->statistics = rightS;
	}

	/**
	 * Return statistics for given input
	 */
	template< typename RDFContext >
	typename RDFContext::StatisticsType& RDFTree<RDFContext>::evaluate( const InputType& input ) const
	{
	  Node* n = root_;
	  while( n->split )
	  {
		if( n->feature( input ) )
		{
		  n = rightChild( n );
		}
		else
		{
		  n = leftChild( n );
		}
	  }
	  return n->statistics;
	}

	template< typename RDFContext >
	XMLNode* RDFTree<RDFContext>::serialize() const
	{
	  XMLElement* serNode_ = new XMLElement( "Tree");

	  String s;
	  s += maxDepth;
	  XMLAttribute* attr = new XMLAttribute( "maxdepth", s );
	  serNode_->addChild( attr );

	  if( root_ )
	  {
		 serNode_->addChild( serializeNode( root_ ) );
	  }

	  return serNode_;
	}

	template< typename RDFContext >
	void RDFTree<RDFContext>::deserialize( XMLNode* node )
	{
	  XMLNode* n = node->childByName( "Node" );
	  if( n )
	  {
		maxDepth = node->childByName( "maxdepth" )->value().toInteger();
		initTree( maxDepth, StatisticsType() );
		deserializeNode( root_, n );
	  }
	}

	template< typename RDFContext >
	std::ostream& operator<<( std::ostream& os, const RDFTree<RDFContext>& tree )
	{
	  tree.preorder( os, tree.root_ );
	  return os;
	}

	template< typename RDFContext >
	void RDFTree<RDFContext>::preorder( std::ostream& os, Node* p, int indent) const
	{
	  if( p != NULL ) {
		if ( indent ) {
		  os << std::setw( indent ) << ' ';
		}
		if( p->isSplit() )
		{
		  os << p->feature << std::endl;

		  preorder( os, leftChild( p ), indent + 2 );

		  preorder( os, rightChild( p ), indent + 2 );
		}
		else
		{
		  os << p->statistics << std::endl;
		}
	  }
	}

	template< typename RDFContext >
	XMLNode* RDFTree<RDFContext>::serializeNode(const Node* node_) const
	{

		XMLElement* nodeSer_ = new XMLElement( "Node");
		XMLElement* elem_;

		if( node_->isSplit() )
		{
			nodeSer_->addChild( node_->feature.serialize() );
			nodeSer_->addChild( serializeNode( leftChild( node_ ) ) );
			nodeSer_->addChild( serializeNode( rightChild( node_ ) ) );
		}
		else
		{
			nodeSer_->addChild( node_->statistics.serialize() );
		}

		return nodeSer_;
	}

	template< typename RDFContext >
	void RDFTree<RDFContext>::deserializeNode( Node* node_, XMLNode* nodeSer_ )
	{
		if( nodeSer_->childSize() == 1 )
		{
			node_->statistics.deserialize( nodeSer_->child( 0 ) );
		}
		else
		{
			node_->feature.deserialize( nodeSer_->child( 0 ) );
			node_->split = true;
			deserializeNode( leftChild( node_ ), nodeSer_->child( 1 ) );
			deserializeNode( rightChild( node_ ), nodeSer_->child( 2 ) );
		}

	}

}

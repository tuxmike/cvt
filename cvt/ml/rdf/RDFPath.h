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

#ifndef CVT_RDFPATH_H
#define CVT_RDFPATH_H

#include <vector>
#include <iostream>

namespace cvt {

	/**
	 * Path in a Binary-Tree
	 */
	class RDFPath
	{
	  public:
		RDFPath();
		RDFPath( size_t path, size_t depth );
		RDFPath( size_t idx );
		RDFPath( const RDFPath& other );
		~RDFPath();

		RDFPath& operator=( const RDFPath& other );

		friend std::ostream& operator<< ( std::ostream& os, const RDFPath& path );

		size_t path() const;
		size_t depth() const;
		size_t index() const;

		bool isBlacklisted( const std::vector< RDFPath >& blacklist ) const;
		void add( bool right );

	  private:
		size_t myPath;
		size_t myDepth;
	};


	inline RDFPath::RDFPath() :
			  myPath( 0 ),
			  myDepth( 0 )
	{}

	/**
	 * Init path
	 * path: bitwise path
	 * depth: tree level
	 */
	inline RDFPath::RDFPath( size_t path, size_t depth ) :
	  myPath( path ),
	  myDepth( depth )
	{}

	/**
	 * Init path
	 * idx: global node index
	 */
	inline RDFPath::RDFPath( size_t idx ) :
	  myPath( 0 ),
	  myDepth( 0 )
	{
		size_t idx_ = idx + 1;
		while ( idx_ >>= 1UL )
		{
			myDepth++;
		}
		myPath = idx - index();
	}

	/**
	 * Copy C'tor
	 */
	inline RDFPath::RDFPath( const RDFPath& other ) :
	  myPath( other.myPath ),
	  myDepth( other.myDepth )
	{}

	inline RDFPath::~RDFPath()
	{}

	/*
	 * Copy
	 */
	inline RDFPath& RDFPath::operator=( const RDFPath& other )
	{
	  if( this != &other )
	  {
		myPath = other.myPath;
		myDepth = other.myDepth;
	  }
	  return *this;
	}

	/**
	 * Debug Output
	 */
	inline std::ostream& operator<<( std::ostream& os, const RDFPath& path )
	{
		os << "Path(" << path.path() << "|" << path.depth() << ")=" << path.index();
		return os;
	}

	/**
	 * Get bitwise path
	 */
	inline size_t RDFPath::path() const
	{
	  return myPath;
	}

	/**
	 * Get depth
	 */
	inline size_t RDFPath::depth() const
	{
	  return myDepth;
	}

	/**
	 * Get global node index
	 */
	inline size_t RDFPath::index() const
	{
		return ( 1UL << myDepth ) - 1  + myPath;
	}

	/**
	 * Check, if blacklisted paths contain path leaf
	 */
	inline bool RDFPath::isBlacklisted( const std::vector< RDFPath >& blacklist ) const
	{
	  for( size_t i = 0; i < blacklist.size(); i++ )
	  {
		const RDFPath& p = blacklist[ i ];
		size_t subpath = myPath >> ( myDepth - p.depth() );
		if( subpath == p.path() )
		{
		  return true;
		}
	  }
	  return false;
	}

	/**
	 * Add leaf
	 */
	inline void RDFPath::add( bool right )
	{
	  myDepth++;
	  myPath <<= 1;
	  if( right )
	  {
		myPath++;
	  }
	}

}

#endif

#ifndef CVT_FONT_H
#define CVT_FONT_H

#include <cvt/geom/Rect.h>

namespace cvt {
	class Font {
		public:
			virtual ~Font() {};
			virtual int size() const = 0;
			virtual Recti stringBounds( const char* str, size_t n = 0 ) = 0;
	};
}

#endif

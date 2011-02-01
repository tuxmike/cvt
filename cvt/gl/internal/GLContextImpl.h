#ifndef CVT_GLCONTEXTIMPL_H
#define CVT_GLCONTEXTIMPL_H

namespace cvt {
	class GLContextImpl {
		public:
			virtual ~GLContextImpl();

			const GLFormat& format() const;
			virtual void makeCurrent() = 0;
			virtual void swapBuffers() = 0;
			virtual void resetCurrent() = 0;

		protected:
			GLContextImpl( const GLFormat& format );

			GLFormat _format;
	};

	inline GLContextImpl::GLContextImpl( const GLFormat& format ) : _format( format )
	{
	}

	inline const GLFormat& GLContextImpl::format() const
	{
		return _format;
	}
}

#endif
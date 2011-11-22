#ifndef CVT_PLOTVIEW_H
#define CVT_PLOTVIEW_H

#include <cvt/gui/Widget.h>
#include <list>

#include "PlotData.h"

namespace cvt {
	class PlotView : public Widget {
		public:
			PlotView();
			~PlotView();

			void			setView( const Rectf& r );
			const Rectf&	view() const;

			void			setXMargin( int value );
			int				xMargin() const;
			void			setYMargin( int value );
			int				yMargin() const;
			void			setXTicsLabelMargin( int value );
			int				xTicsLabelMargin() const;
			void			setYTicsLabelMargin( int value );
			int				yTicsLabelMargin() const;

			void			setXTics( float xtics );
			float			xTics() const;
			void			setYTics( float xtics );
			float			yTics() const;
			void			setGrid( bool b );
			bool			grid() const;
			void			setXTicsLabel( const String& xticslabel );
			const String&	xTicsLabel() const;
			void			setYTicsLabel( const String& yticslabel );
			const String&	yTicsLabel() const;
			void			setXLabel( const String& xlabel );
			const String&	xLabel() const;
			void			setYLabel( const String& xlabel );
			const String&	yLabel() const;

			void			addPlotData( const PlotData* pdata );
			void			removePlotData( const PlotData* pdata );
			size_t			plotDataSize() const;

			void paintEvent( PaintEvent* pe, GFX* g);

		private:
			int				_xmargin, _ymargin;
			int				_xticslabelmargin, _yticslabelmargin;
			Rectf			_view;
			float			_xtics, _ytics;
			bool			_grid;
			String			_xticslabel, _yticslabel;
			String			_xlabel, _ylabel;

			std::list<const PlotData*> _pdata;
	};

	inline void PlotView::setXMargin( int value )
	{
		if( _xmargin == value )
			return;
		_xmargin = value;
		update();
	}

	inline int PlotView::xMargin() const
	{
		return _xmargin;
	}

	inline void PlotView::setYMargin( int value )
	{
		if( _ymargin == value )
			return;
		_ymargin = value;
		update();
	}

	inline int PlotView::yMargin() const
	{
		return _ymargin;
	}

	inline void PlotView::setXTicsLabelMargin( int value )
	{
		if( _xticslabelmargin == value )
			return;
		_xticslabelmargin = value;
		update();
	}

	inline int PlotView::xTicsLabelMargin() const
	{
		return _xticslabelmargin;
	}

	inline void PlotView::setYTicsLabelMargin( int value )
	{
		if( _yticslabelmargin == value )
			return;
		_yticslabelmargin = value;
		update();
	}

	inline int PlotView::yTicsLabelMargin() const
	{
		return _yticslabelmargin;
	}

	inline void PlotView::setView( const Rectf& r )
	{
		if( _view == r )
			return;
		_view = r;
		update();
	}

	inline const Rectf& PlotView::view() const
	{
		return _view;
	}

	inline void PlotView::setXTics( float xtics )
	{
		if( _xtics == xtics )
			return;
		_xtics = xtics;
		update();
	}

	inline float PlotView::xTics() const
	{
		return _xtics;
	}

	inline void PlotView::setYTics( float ytics )
	{
		if( _ytics == ytics )
			return;
		_ytics = ytics;
		update();
	}

	inline float PlotView::yTics() const
	{
		return _ytics;
	}

	inline void PlotView::setGrid( bool b )
	{
		if( _grid == b)
			return;
		_grid = b;
		update();
	}

	inline bool PlotView::grid() const
	{
		return _grid;
	}

	inline void PlotView::setXTicsLabel( const String& label )
	{
		if( _xticslabel == label )
			return;
		_xticslabel = label;
		update();
	}

	inline const String& PlotView::xTicsLabel() const
	{
		return _xticslabel;
	}

	inline void PlotView::setYTicsLabel( const String& label )
	{
		if( _yticslabel == label )
			return;
		_yticslabel = label;
		update();
	}

	inline const String& PlotView::yTicsLabel() const
	{
		return _yticslabel;
	}

	inline void PlotView::setXLabel( const String& label )
	{
		if( _xlabel == label )
			return;
		_xlabel = label;
		update();
	}

	inline const String& PlotView::xLabel() const
	{
		return _xlabel;
	}

	inline void PlotView::setYLabel( const String& label )
	{
		if( _ylabel == label )
			return;
		_ylabel = label;
		update();
	}

	inline const String& PlotView::yLabel() const
	{
		return _ylabel;
	}

	inline void PlotView::addPlotData( const PlotData* pdata )
	{
		_pdata.push_back( pdata );
		update();
	}

	inline void PlotView::removePlotData( const PlotData* pdata )
	{
		_pdata.remove( pdata );
		update();
	}

	inline size_t PlotView::plotDataSize() const
	{
		return _pdata.size();
	}

}

#endif
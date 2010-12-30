#ifndef CVT_FEATURE_VIEW_H
#define CVT_FEATURE_VIEW_H

#include <cvt/gui/ImageView.h>
#include <cvt/vision/FeatureExtractor.h>
#include <cvt/math/Vector.h>

class FeatureView : public cvt::ImageView
{
	public:
		FeatureView();
		~FeatureView();

		void setFeatures( std::vector<cvt::Feature2D> & features );

	private:
		std::vector<cvt::Feature2D> _features;
	protected:
		void paintEvent( cvt::PaintEvent* ev, cvt::GFX* g );
};

inline FeatureView::FeatureView() : cvt::ImageView(), _features( 0 )
{
}

void FeatureView::paintEvent( cvt::PaintEvent* ev, cvt::GFX* g )
{
	cvt::ImageView::paintEvent( ev, g );

	g->setColor( cvt::Color( 0.0f, 1.0f, 0.0f, 0.5f ) );

	int w, h;
	size( w, h );
	cvt::Vector2i* pts = new cvt::Vector2i[ _features.size() ];
	float xScale = ( float ) w / 640.0f;
	float yScale = ( float ) h / 480.0f;
	for( size_t i = 0; i < _features.size(); i++ ){
		pts[ i ].x = ( int ) ( _features[ i ][ 0 ] * xScale - 8.0f );
		pts[ i ].y = ( int ) ( _features[ i ][ 1 ] * yScale - 8.0f );
	}
	//g->drawIcon( _features[ i ][ 0 ] * xScale - 8.0f, _features[ i ][ 1 ] * yScale -8.0f, cvt::GFX::ICON_CROSS );
	g->drawIcons( pts, _features.size(), cvt::GFX::ICON_CROSS );
	delete[] pts;
}

inline FeatureView::~FeatureView()
{
}

inline void FeatureView::setFeatures( std::vector<cvt::Feature2D> & features )
{
	_features.clear();
	_features.resize( features.size() );
	for( size_t i = 0; i < features.size(); i++ ){
		_features[ i ][ 0 ] = features[ i ][ 0 ];
		_features[ i ][ 1 ] = features[ i ][ 1 ];
	}
}

#endif
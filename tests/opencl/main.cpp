#include <cvt/cl/CLContext.h>
#include "CLOptflow.h"
#include <cvt/gfx/Image.h>
#include <cvt/gfx/Color.h>
#include <cvt/gfx/IFilterScalar.h>
#include <cvt/io/ImageIO.h>
#include <cvt/util/Exception.h>
#include <cvt/util/Timer.h>
#include <cvt/io/FloFile.h>
#include <cvt/vision/Flow.h>
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <cvt/io/V4L2Camera.h>

#include "calcp1.h"
#include "calcp2.h"
#include "clearimg.h"
#include "copyimg.h"
#include "bilindown.h"
#include "warp.h"
#include "warpsub.h"
#include "grad.h"

using namespace cvt;

int main( int argc, char** argv )
{
	std::string log;
	int key;
	std::vector<cl::Event> sync;
	cl::Event event;
	int pyridx = 0;
	CLImage* u;
	Image img1, img2, in1, in2;

	try {
		CLContext cl;
		cl.makeCurrent();
		CLOptflow flow;

		ImageIO::loadPNG( img1, argv[ 1 ] );
		ImageIO::loadPNG( img2, argv[ 2 ] );

		in1.reallocate( img1.width(), img1.height(), CVT_GRAY, CVT_FLOAT );
		in2.reallocate( img2.width(), img2.height(), CVT_GRAY, CVT_FLOAT );
		img1.convert( in1, CVT_GRAY, CVT_FLOAT );
		img2.convert( in2, CVT_GRAY, CVT_FLOAT );

		Image iflow( 640, 480, CVT_RGBA, CVT_UBYTE );
		u = flow.updateFlow( in1 );
		delete u;
		u = flow.updateFlow( in2 );
		u->readData( iflow.data(), iflow.stride() );
		delete u;


		while( 1 ) {
			cvShowImage( "Video", iflow.iplimage() );

			key = cvWaitKey( 10 ) & 0xff;
			if( key == 27 )
				break;

		}
	} catch( CLException e ) {
		std::cout << e.what() << std::endl;
	} catch( Exception e ) {
		std::cout << e.what() << std::endl;
	}
}


#include <cvt/ml/rdf/RDFClassificationTrainer2D.h>
#include <cvt/ml/rdf/RDFClassifier.h>

#include <cvt/geom/Rect.h>

#include <cvt/math/Vector.h>

#include <cvt/util/String.h>
#include <cvt/util/Data.h>
#include <cvt/util/DataIterator.h>
#include <cvt/io/FileSystem.h>

using namespace cvt;

int main( int argc, char** argv )
{
	RDFClassificationTrainer2D rdftrainer( 2 );

    Data filedata;
    FileSystem::load( filedata, argv[ 1 ] );
	std::vector<Vector3f> traindata;
	Rectf rect;

    DataIterator iter( filedata );
	iter.skip( "\t\n\r " );
	while( iter.hasNext() ) {
		Vector3f data;
		data.z = iter.nextDouble() - 1.0f;
		data.x = iter.nextDouble();
		data.y = iter.nextDouble();
		rect.join( data.x, data.y );
		traindata.push_back( data );
		iter.skip( "\t\n\r " );
	};

	srand( time( NULL ) );
	RDFClassifier<Vector2f,2> rdfclassifier;
	for( size_t i = 0; i < 200; i++ ) {
		rdfclassifier.addTree( rdftrainer.train( traindata, 6, 20000 ) );
	}

	rect.grow( 200.0f );
	Image img;
	RDFClassificationTrainer2D::visualizeClassifier( img, rdfclassifier, rect, 800.0f, 800.0f * ( rect.height / rect.width ) );
	img.save("rdf2d.png");
//	for( size_t i = 0; i < traindata.size(); i++ ) {
//		Vector3f tdata = traindata[ i ];
//		Vector2f pos( tdata.x, tdata.y );
//		std::cout << "GT: " << tdata.z << std::endl;
//		RDFClassHistogram<2> chist;
//		rdfclassifier.classify( chist, pos );
//		std::cout <<  chist << std::endl;
//	}


	return 0;
}
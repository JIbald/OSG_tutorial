#include <osg/Geometry>
#include <osg/Geode>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>

int main( int argc, char** argv )
{
	//cube vertices
	osg::ref_ptr <osg::Vec3Array> verticesCube = new osg::Vec3Array(8);
	//bottom square
	(*verticesCube)[0].set( 0.0f, 0.0f, 0.0f );
	(*verticesCube)[1].set( 1.0f, 0.0f, 0.0f );
	(*verticesCube)[2].set( 0.0f, 1.0f, 0.0f );
	(*verticesCube)[3].set( 1.0f, 1.0f, 0.0f );
	//top square
	(*verticesCube)[4].set( 0.0f, 0.0f, 1.0f );
	(*verticesCube)[5].set( 1.0f, 0.0f, 1.0f );
	(*verticesCube)[6].set( 0.0f, 1.0f, 1.0f );
	(*verticesCube)[7].set( 1.0f, 1.0f, 1.0f );

	//pyramid vertices
	osg::ref_ptr <osg::Vec3Array> verticesPyr = new osg::Vec3Array(5); //TODO: add size
	//bottom square
	(*verticesPyr)[0].set( 0.0f, 0.0f, 1.0f );
	(*verticesPyr)[1].set( 1.0f, 0.0f, 1.0f );
	(*verticesPyr)[2].set( 0.0f, 1.0f, 1.0f );
	(*verticesPyr)[3].set( 1.0f, 1.0f, 1.0f );
	//apex
	(*verticesPyr)[4].set( 0.5f, 0.5f, 2.0f );

	//cube indices
	osg::ref_ptr <osg::DrawElementsUInt> indicesCube = new osg::DrawElementsUInt( GL_TRIANGLES, 36 );

	//bottom-back-left corner
	(*indicesCube)[0] = 0;		(*indicesCube)[1] = 1;		(*indicesCube)[2] = 2;
	(*indicesCube)[3] = 0;		(*indicesCube)[4] = 1;		(*indicesCube)[5] = 4;
	(*indicesCube)[6] = 0; 		(*indicesCube)[7] = 2;		(*indicesCube)[8] = 4;
	//bottom-front-right corner 
	(*indicesCube)[9] = 1;		(*indicesCube)[10] = 2;		(*indicesCube)[11] = 3;
	(*indicesCube)[12] = 1; 	(*indicesCube)[13] = 3;		(*indicesCube)[14] = 7;
	(*indicesCube)[15] = 2;		(*indicesCube)[16] = 3;		(*indicesCube)[17] = 7;
	//top-front-left corner
	(*indicesCube)[18] = 4;		(*indicesCube)[19] = 6;		(*indicesCube)[20] = 7;
	(*indicesCube)[21] = 4; 	(*indicesCube)[22] = 2;		(*indicesCube)[23] = 6;
	(*indicesCube)[24] = 2;		(*indicesCube)[25] = 6;		(*indicesCube)[26] = 7;
	//top-back-left corner
	(*indicesCube)[27] = 4;		(*indicesCube)[28] = 5;		(*indicesCube)[29] = 7;
	(*indicesCube)[30] = 4; 	(*indicesCube)[31] = 1;		(*indicesCube)[32] = 5;
	(*indicesCube)[33] = 5;		(*indicesCube)[34] = 1;		(*indicesCube)[35] = 7;	
	
	//pyramid indices
	osg::ref_ptr <osg::DrawElementsUInt> indicesPyr = new osg::DrawElementsUInt( GL_QUADS, 18 );
	//back
	(*indicesPyr)[0] = 0;		(*indicesPyr)[1] = 1;		(*indicesPyr)[2] = 2;//floor
	(*indicesPyr)[3] = 0;		(*indicesPyr)[4] = 2;		(*indicesPyr)[5] = 4;
	(*indicesPyr)[6] = 0;		(*indicesPyr)[7] = 1;		(*indicesPyr)[8] = 4;
	//front
	(*indicesPyr)[9] = 1;		(*indicesPyr)[10] = 2;		(*indicesPyr)[11] = 3;//floor
	(*indicesPyr)[12] = 1;		(*indicesPyr)[13] = 3;		(*indicesPyr)[14] = 4;
	(*indicesPyr)[15] = 2;		(*indicesPyr)[16] = 3;		(*indicesPyr)[17] = 4;

	//Cube: attach vertices and index	
	osg::ref_ptr <osg::Geometry> geomCube = new osg::Geometry;
	geomCube -> setVertexArray( verticesCube.get() );
	geomCube -> addPrimitiveSet( indicesCube.get() );
	osgUtil::SmoothingVisitor::smooth( *geomCube );
	
	//pyramid: attach vertices and index
	osg::ref_ptr <osg::Geometry> geomPyr = new osg::Geometry;
	geomPyr -> setVertexArray( verticesPyr.get() );
	geomPyr -> addPrimitiveSet( indicesPyr.get() );
	osgUtil::SmoothingVisitor::smooth( *geomPyr );

	osg::ref_ptr <osg::Geode> root = new osg::Geode;
	//root -> addDrawable( geomCube.get() );
	root -> addDrawable( geomPyr.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

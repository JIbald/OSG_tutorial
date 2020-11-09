
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/TriangleFunctor>
#include <osgUtil/Tessellator>
#include <osgViewer/Viewer>
#include <iostream>

void outVec3 ( osg::Vec3 vec3 )
{
	for( int i {0}; i < vec3.length(); ++i )
	{
		std::cout << vec3[i] << " ";
	}
}

struct FaceCollector
{
	//TODO: Nani the fuck is this bool there?
	//let's comment it out for now
	void operator() ( const osg::Vec3 &v1, const osg::Vec3 &v2, const osg::Vec3 &v3) // ,bool )
	{
		std::cout << "Face vertices: " << std::endl;
		outVec3(v1);
		std::cout << std::endl;
		outVec3(v2);
		std::cout << std::endl;
		outVec3(v3);
		std::cout << std::endl;
	}
};

int main( int argc, char** argv )
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices -> push_back( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
	vertices -> push_back( osg::Vec3( 1.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 1.0f, 0.0f, 1.5f ) );
	vertices -> push_back( osg::Vec3( 2.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 2.0f, 0.0f, 1.0f ) );
	vertices -> push_back( osg::Vec3( 3.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 3.0f, 0.0f, 1.5f ) );
	vertices -> push_back( osg::Vec3( 4.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 4.0f, 0.0f, 1.0f ) );

	osg::ref_ptr <osg::Vec3Array> normals = new osg::Vec3Array;
	normals -> push_back( osg::Vec3( 0.0f, -1.0f, 0.0f ) );

	osg::ref_ptr <osg::Geometry> geom = new osg::Geometry;
	geom -> setVertexArray( vertices.get() );
	geom -> setNormalArray( normals.get() );
	geom -> setNormalBinding( osg::Geometry::BIND_OVERALL );
	geom -> addPrimitiveSet( new osg::DrawArrays( GL_QUAD_STRIP, 0, 10 ) );
	
	osg::TriangleFunctor <FaceCollector> functor;
	geom -> accept(functor);

	osg::ref_ptr <osg::Geode> root = new osg::Geode;
	root -> addDrawable( geom.get() );	

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	viewer.run();

}

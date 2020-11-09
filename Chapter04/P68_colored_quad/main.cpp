#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/Viewer>

int main( int argc, char** argv )
{
	//create vertex array push_back four corner points
	osg::ref_ptr <osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices -> push_back( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 1.0f, 0.0f, 0.0f ) );
	vertices -> push_back( osg::Vec3( 1.0f, 0.0f, 1.0f ) );
	vertices -> push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

	//indicate normal of each vertex, since default normal can cause lighting problems
	osg::ref_ptr <osg::Vec3Array> normals = new osg::Vec3Array;
	normals -> push_back( osg::Vec3( 0.0f, -1.0f, 0.0f ) );

	//indicate color to each vertex, OpenGL default: smooth coloring, blends colors 
	osg::ref_ptr <osg::Vec4Array> colors = new osg::Vec4Array;
	colors -> push_back( osg::Vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ) );
	colors -> push_back( osg::Vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ) );
	colors -> push_back( osg::Vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ) );
	colors -> push_back( osg::Vec4 ( 1.0f, 1.0f, 1.0f, 1.0f ) );

	//create osg::Geometry obj, set prepared vertex, normal, color.
	//indicate that the single normal bounds to entire geometry
	//and colors bound per vertex
	osg::ref_ptr <osg::Geometry> quad = new osg::Geometry;
	quad -> setVertexArray( vertices.get() );
	quad -> setNormalArray( normals.get() );
	quad -> setNormalBinding( osg::Geometry::BIND_OVERALL );
	quad -> setColorArray ( colors.get() );
	quad -> setColorBinding ( osg::Geometry::BIND_PER_VERTEX );

	//specify the primitive set.
	//newly allocated osg::Draw Arrays instance w/ drawing mode set to GL_QUADS
	//to render four vertices as quad corners in counter-clockwise order:
	quad -> addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4 ) );

	//add geometry to osg::Geode obj and render
	osg::ref_ptr <osg::Geode> root = new osg::Geode;
	root -> addDrawable( quad.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}	


// common translucent effect (glass)
// other object can be displayed through this object
// -> OpenGL blending, but with correctly calculated rendering order

#include <osg/BlendFunc>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main( int argc, char** argv)
{
	// quad geometry
	// predefined texture coord array
	// treated as translucent object
	// apply blending attribute and modes later
	
	osg::ref_ptr <osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices -> push_back( osg::Vec3( -0.5f, 0.0f, -0.5f ) );
	vertices -> push_back( osg::Vec3(  0.5f, 0.0f, -0.5f ) );
	vertices -> push_back( osg::Vec3(  0.5f, 0.0f,  0.5f ) );
	vertices -> push_back( osg::Vec3( -0.5f, 0.0f,  0.5f ) );

	osg::ref_ptr <osg::Vec3Array> normals = new osg::Vec3Array;
	normals -> push_back( osg::Vec3( 0.0f, -1.0f, 0.0f ) );

	osg::ref_ptr <osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords -> push_back( osg::Vec2( 0.0f, 0.0f ) );
	texcoords -> push_back( osg::Vec2( 0.0f, 1.0f ) );
	texcoords -> push_back( osg::Vec2( 1.0f, 1.0f ) );
	texcoords -> push_back( osg::Vec2( 1.0f, 0.0f ) );

	// CAREFUL: set color array of quad
	// to blend it with other scene objects,
	// we have to set the alpha component to a value of less than 1.0
	osg::ref_ptr <osg::Vec4Array> colors = new osg::Vec4Array;
	colors -> push_back ( osg::Vec4( 1.0f, 1.0f, 1.0f, 0.5f ) );

	// quad geometry
	osg::ref_ptr <osg::Geometry> quad = new osg::Geometry;
	quad -> setVertexArray( vertices.get() );
	quad -> setNormalArray( normals.get() );
	quad -> setNormalBinding( osg::Geometry::BIND_OVERALL );
	quad -> setColorArray( colors.get() );
	quad -> setColorBinding( osg::Geometry::BIND_OVERALL );
	quad -> setTexCoordArray( 0, texcoords.get() );
	quad -> addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4 ) );

	osg::ref_ptr <osg::Geode> geode = new osg::Geode;
	geode -> addDrawable( quad.get() );

	// apply texture to quad
	osg::ref_ptr <osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr <osg::Image> image = osgDB::readImageFile( "Images/lz.rgb" );
	texture -> setImage( image.get() );

	// osg::BlendFunc class to implenent blending effect
	// works like OpenGL's glBlendFunc()
	osg::ref_ptr <osg::BlendFunc> blendFunc = new osg::BlendFunc;
	blendFunc -> setFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// blend function attribute and texture attribute to state set
	osg::StateSet* stateset = geode -> getOrCreateStateSet();
	stateset -> setTextureAttributeAndModes( 0, texture.get() );
	stateset -> setAttributeAndModes( blendFunc );
	stateset -> setRenderingHint( osg::StateSet::TRANSPARENT_BIN );			

	//adding geometry node and a loaded glider moedel to the sg
	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( geode.get() );
	root -> addChild( osgDB::readNodeFile( "glider.osg" ) );
	
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();

}

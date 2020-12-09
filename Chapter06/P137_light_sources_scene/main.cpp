// by default, OSG automatically turns on the first light (GL_LIGHT0)
// -> gives scene soft, directional light
//
// however, this time we create multiple lights
// -> move them w/ transformation parent nodes
// !! only positional lights can be translated
// -> directional light has no origin and can't be placed anywhere
//
// OpenGL and OSG both use 4th component of the position param to decide,
// if a light is a point light. 
// -> if the 4th component is 0, light is treated as a directional source
// 	-> otherwise it's positional

#include <osg/MatrixTransform>
#include <osg/LightSource>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

// creates light sources for sg
	// 	-> light source should have a number from 0 to 7
	// 	-> a translation position
	// 	-> color param
	// -> point light is created because the fourth part of the pos vector is 1.0
	// 	-> after that, we assign the light to newly-created osg::LightSource node
	// 	-> add light source to translated osg::MatrixTransform node
	// 		-> which is returned
	osg::Node* createLightSource( unsigned int num, const osg::Vec3& trans, const osg::Vec4& color )
	{
		osg::ref_ptr <osg::Light> light = new osg::Light;
		light -> setLightNum( num );
		light -> setDiffuse( color );
		light -> setPosition( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

		osg::ref_ptr <osg::LightSource> lightSource = new osg::LightSource;
		lightSource -> setLight( light );

		osg::ref_ptr <osg::MatrixTransform> sourceTrans = new osg::MatrixTransform;
		sourceTrans -> setMatrix( osg::Matrix::translate( trans ) );
		sourceTrans -> addChild( lightSource.get() );

		return sourceTrans.release();
	}

int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );

	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( model.get() );

	// construct 2 light source nodes and their positions
	osg::Node* light0 = createLightSource( 0, osg::Vec3( -20.0f, 0.0f, 0.0f ), 
						  osg::Vec4( 1.0f, 0.0f, 1.0f, 1.0f ) );
	osg::Node* light1 = createLightSource( 1, osg::Vec3( 0.0f, -20.0f, 0.0f ),
						  osg::Vec4( 0.0f, 1.0f, 1.0f, 1.0f ) );
	
	// light numbers 0 and 1 are used
	// -> we turn on modes GL_LIGHT0 and GL_LIGHT1 of root node
	// 	-> means all nodes in sg could benefit from the two warm light sources:
	root -> getOrCreateStateSet() -> setMode( GL_LIGHT0, osg::StateAttribute::ON );
	root -> getOrCreateStateSet() -> setMode( GL_LIGHT1, osg::StateAttribute::ON );
	root -> addChild( light0 );
	root -> addChild( light1 );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
	
}

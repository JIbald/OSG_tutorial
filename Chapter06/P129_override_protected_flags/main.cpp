// OVERRIDE and PROTECTED usage 
// root nood will be set to OVERRIDE to force all children to inherit attribute or mode
// children will try to change their inheritance with or without a PROTETED flag
// -> different results

#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );

	osg::ref_ptr <osg::MatrixTransform> transformation1 = new osg::MatrixTransform;
	transformation1 -> setMatrix( osg::Matrix::translate( -25.0f, 0.0f, 0.0f ) );
	transformation1 -> addChild( model.get() );
	
	osg::ref_ptr <osg::MatrixTransform> transformation2 = new osg::MatrixTransform;
	transformation2 -> setMatrix( osg::Matrix::translate( 25.0f, 0.0f, 0.0f ) );
	transformation2 -> addChild( model.get() );

	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( transformation1.get() );
	root -> addChild( transformation2.get() );
	
	// set rendering mode of each node's state set.
	// GL_LIGHTING OpenGL enum is used to enable/disable global lighting of scene
	// OVERRIDE and PROTECTED flags are set to root and transformation2 separately,
	// along with ON or OFF switch value
	transformation1 -> getOrCreateStateSet() -> setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	transformation2 -> getOrCreateStateSet() -> setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	root -> getOrCreateStateSet() -> setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

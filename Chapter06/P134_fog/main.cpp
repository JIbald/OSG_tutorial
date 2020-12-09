// fog example of working with various rendering attrbutes and modes.
// OpenGL accepts
// 	one linear fog equation
// 	two exponential fog equations
// 	-> both are supported by the osg::Fog class

#include <osg/Fog>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main ( int argc, char** argv )
{
	// create fog attribute
	// linear mode -> set near and far distances w/ setStart() / setEnd()
	// set fog color to create dust fog-like effects
	osg::ref_ptr <osg::Fog> fog = new osg::Fog;
	fog -> setMode( osg::Fog::LINEAR );
	fog -> setStart( 500.0f );
	fog -> setEnd( 2500.0f );
	fog -> setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

	// load terrain model
	// set fog attribute and associated mode to node's state set
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "lz.osg" );
	model -> getOrCreateStateSet() -> setAttributeAndModes( fog.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( model.get() );
	return viewer.run();
}

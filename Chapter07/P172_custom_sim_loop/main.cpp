// using while statement
// display frame number
// custom sim loop cannot benefit from on-demand rendering schemeand max frame rate setting.
// -> only available when using the run() method

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <iostream>

int main (int argc, char** argv)
{
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "lz.osg" );
	osgViewer::Viewer viewer;
	viewer.setSceneData( model.get() );

	// set manip to viewer
	// -> otherwise unable to navigate, zoom, pan, orbit, ...
	// osgGA::TrackballManipulator is the default manip used internally in the run() method:
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );
	
	viewer.setUpViewInWindow( 710, 290, 500, 500, 0 );

	// while loop
	// condition is tested every frame by using done()
	// frame() executes every frame to update, cull and render the sg
	while ( !viewer.done() )
	{
		viewer.frame();
		std::cout << "Frame number: " << viewer.getFrameStamp() -> getFrameNumber() 
			  << std::endl;
	}
	return 0;
}

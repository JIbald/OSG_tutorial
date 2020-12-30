// multisampling is a type of anti-aliasing technique
// can improve the final tesult's quality without much performance hit.
// User apps should set a sampling number for implementing multisampe rasterization.
// -> not all gpus support the multisampling extension
// -> this may fail on some systems

#include <osg/DisplaySettings>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main( int argc, char** argv)
{
	// set the number of muyltisamples.
	// available values often include:
	// 2, 4, 6 
	// depending on gpu
	osg::DisplaySettings::instance() -> setNumMultiSamples( 4 );

	// Load a model + render w/ standard viewer
	// global multisampling attribute managed by the osg::DisplaySettings singleton
	// has already come into effect now
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );
	osgViewer::Viewer viewer;
	viewer.setSceneData( model.get() );
	return viewer.run();
}

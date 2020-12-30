// with OSG's internal anaglyph stereo mode we implement a simple and quick steroscopic 3D effect.
// prepare a pair of 3D red/cyan glasses

#include <osg/DisplaySettings>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>


int main( int argc, char** argv )
{
	// direct work on the global display settings
	// There are three steps to follow:
	// switch the stereo mode to ANAGLYPHIC, set a suitable eye separation (distance from left to right eye )
	// with setEyeSeparation() method
	// enable stereo visualization
	osg::DisplaySettings::instance() -> setStereoMode( osg::DisplaySettings::ANAGLYPHIC );
	osg::DisplaySettings::instance() -> setEyeSeparation( 0.05f );
	osg::DisplaySettings::instance() -> setStereo( true );

	// we construct and render sg
	// cessna model is simple enough.
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );
	osgViewer::Viewer viewer;
	viewer.setSceneData( model.get() );
	return viewer.run();
}

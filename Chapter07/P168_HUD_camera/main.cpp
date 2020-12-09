// HUD  can render data without requiring users to look away from their usual viewpoint.
// widely used in 3d scenes
// -> displaying important 2D text,
// -> computer game statistics
// -> flight and cockpit instruments
//
// We design a HUD camera, which contains a model that should be placed in front of other scene objects
// at any time.

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>


int main( int argc, char** argv )
{
	// two models loaded from disk
	// lz.osg demos a terrain\
	// glider.osg will be put under a HUD camera.
	// -> will alwaysbe visible to viewers who are looking ahead
	// 	-> no matter how other parts of sg are changing.
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "lz.osg" );
	osg::ref_ptr <osg::Node> hud_model = osgDB::readNodeFile( "glider.osg" );

	// HUD cam and its children must be rendered AFTER the regular scene
	// is finished being drawn on screen.
	// -> it will overwrite all present pixel data 
	// 	(regardless of its location and depth.)
	// -> that's why we use GL_DEPTH_BUFFER_BIT to clear the depth buffer.
	// -> GL_COLOR_BUFFER_BIT is not set here, to ensure the color buffer's correct reservation
	osg::ref_ptr <osg::Camera> camera = new osg::Camera;
	camera -> setClearMask( GL_DEPTH_BUFFER_BIT );
	camera -> setRenderOrder( osg::Camera::POST_RENDER );

	// HUD cam should'nt be affected by the viewer or any other parent nodes
	// -> it needs to be changed to the absolute reference frame
	// -> be set as a custom fixed view matrix
	// glider is also added to the camera node, used as the content to be displayed
	camera -> setReferenceFrame( osg::Camera::ABSOLUTE_RF );
	camera -> setViewMatrixAsLookAt( osg::Vec3( 0.0f, -5.0f, 5.0f ),
					 osg::Vec3(),
					 osg::Vec3( 0.0f, 1.0f, 1.0f )
				       );
	camera -> addChild( hud_model.get() );

	//add the HUD camera, along with a regular loaded model to the root node
	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( model.get() );
	root -> addChild( camera.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

/*
 * This time we are going to display a Cessna in the 3D space and provide descriptive texts in
front of the rendered scene. A heads-up display (HUD) camera can be used here, which is
rendered after the main camera, and only clears the depth buffer for directly updating texts
to the frame buffer. The HUD camera will then render its child nodes in a way that is always
visible.
 */

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgText/Font>
#include <osgText/Text>
#include <osgViewer/Viewer>

/*
 * The osgText::readFontFile() function is used for reading a suitable font
file, for instance, an undistorted TrueType font. The OSG data paths (specified
with OSG_FILE_PATH ) and the windows system path will be searched to see
if the specified file exists:
 */

osg::ref_ptr <osgText::Font> g_font = osgText::readFontFile( "fonts/arial.ttf" );

/*
 * Create a standard HUD camera and set a 2D orthographic projection
matrix for the purpose of drawing 3D texts in two dimensions. The
camera should not receive any user events, and should never be
affected by any parent transformations. These are guaranteed by the
setAllowEventFocus() and setReferenceFrame() methods:
 */
osg::Camera* createHUDCamera( double left, double right, double bottom, double top )
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera -> setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	camera -> setClearMask( GL_DEPTH_BUFFER_BIT );
	camera -> setRenderOrder( osg::Camera::POST_RENDER );
	camera -> setAllowEventFocus( false );
	camera -> setProjectionMatrix(
			osg::Matrix::ortho2D( left, right, bottom, top)
				     );
	return camera.release();
}

/*
 * The text is created by a separate global function, too. It defines a font object
describing every character's glyph, as well as the size and position parameters
in the world space, and the content of the text. In the HUD text implementation,
texts should always align with the XOY plane:
 */
osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
{
	osg::ref_ptr <osgText::Text> text = new osgText::Text;
	text -> setFont( g_font.get() );
	text -> setCharacterSize( size );
	text -> setAxisAlignment( osgText::TextBase::XY_PLANE );
	text -> setPosition( pos );
	text -> setText( content );
	return text.release();
}

/*
 * In the main entry, we create a new osg::Geode node and add multiple text
objects to it. These introduce the leading features of a Cessna. Of course, you
can add your own explanations about this type of monoplane by using additional
osgText::Text drawables:
 */
int main( int argc, char** argv)
{
	osg::ref_ptr <osg::Geode> textGeode = new osg::Geode;
	textGeode -> addDrawable( createText( osg::Vec3( 150.0f, 500.0f, 0.0f ),
					      "The Cessna monoplane",
					      20.0f ) );
	textGeode -> addDrawable( createText( osg::Vec3( 150.0f, 450.0f, 0.0f ),
					      "Six-seat, low-wing and twin-engined",
					      15.0f ) );

	/*
	 * The node including all texts should be added to the HUD camera. To ensure that
	 * the texts won't be affected by OpenGL normals and lights (they are textured
	 * geometries, after all), we have to disable lighting for the camera node:
	 */
	osg::Camera* camera = createHUDCamera( 0, 1024, 0, 768 );
	camera -> addChild( textGeode.get() );
	camera -> getOrCreateStateSet() -> setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( osgDB::readNodeFile( "cessna.osg" ) );
	root -> addChild( camera );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}













































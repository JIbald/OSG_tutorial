/*
 * A simple 3D text object will be created in this example. Like the 2D text class
osgText::Text , the osgText::Text3D class also inherits a list of methods to set basic
text parameters, including position, size, alignment, font object, and the content. 3D texts
are most likely to be used as a special effect of games and applications.
 */
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgText/Font3D>
#include <osgText/Text3D>
#include <osgViewer/Viewer>

/*
 * Read an appropriate font file with the osgText::readFont3DFile() function,
which is similar to osgText::readFontFile() . Using the osgdb_freetype
plugin, TrueType fonts can be parsed into finely-detailed 3D character glyphs:
 */
osg::ref_ptr <osgText::Font3D> g_font3D = osgText::readFont3DFile( "fonts/arial.ttf" );

/*
 *So we are going to imitate the createText() function in the last example.
The only difference is that we have to set an extra depth parameter for the text
character to make it stand out in the 3D world. The setAxisAlignment() method
here indicates that the text object is placed on the XOZ plane, with its front faces
facing the negative Y axis:
 */
osgText::Text3D* createText3D( const osg::Vec3& pos, const std::string& content, float size, float depth )
{
	osg::ref_ptr<osgText::Text3D> text = new osgText::Text3D;
	text -> setFont( g_font3D.get() );
	text -> setCharacterSize( size );
	text -> setCharacterDepth( depth );
	text -> setAxisAlignment( osgText::TextBase::XZ_PLANE );
	text -> setPosition( pos );
	text -> setText( content );
	return text.release();
}

/*
 * Create a 3D text object with short words. Note that because 3D texts are actually
made up of vertices and geometry primitives, abuse of them may cause high
resource consumption:
 */
int main( int argc, char** argv )
{
	osg::ref_ptr <osg::Geode> textGeode = new osg::Geode;
	textGeode -> addDrawable( createText3D(osg::Vec3(), "The Cessna", 20.0f, 10.0f) );

	/*
	 * This time we add an osg::MatrixTransform as the parent of textGeode .
	 * It will apply an additional transformation matrix to the model-view matrix
	 * when renderng all text drawables, and thus change their displayed positions and
	 * attitudes in the world coordinates:
	 */
	osg::ref_ptr <osg::MatrixTransform> textNode = new osg::MatrixTransform;
	textNode -> setMatrix( osg::Matrix::translate( 0.0f, 0.0f, 10.0f ) );
	textNode -> addChild( textGeode.get() );

	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( osgDB::readNodeFile( "cessna.osg" ) );
	root -> addChild( textNode.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

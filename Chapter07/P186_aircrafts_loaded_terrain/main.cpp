#include <osg/Camera>
#include <osg/StateAttribute>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

// look for any textures applied to a loaded model.
// we have to derive a FindTextureVisitor from the osg::NodeVisitor base class.
// This manages a texture object, which will be used for render-to-textures operations later.
// every time we find an existing texture in the sg, we replace it with the managed one.
// -> replaceTexture() method
class FindTextureVisitor : public osg::NodeVisitor
{
public:
	FindTextureVisitor( osg::Texture* tex ) : _texture( tex )
	{
      		setTraversalMode( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	}
	
	virtual void apply( osg::Node& node );
	virtual void apply( osg::Geode& geode );
	virtual void apply( osg::StateSet* ss );

protected:
	osg::ref_ptr <osg::Texture> _texture;

};

// in the apply() method, call replaceTexture() on each node and drawable
// -> see if there are any textures stored. ODn't forget to call traverse()
// -> traverse() continues traversal through sg.
void FindTextureVisitor::apply( osg::Node& node )
{
	replaceTexture( node.getStateSet() );
	traverse( node );
}

void FindTextureVisitor::apply( osg::Geode& geode )
{
	replaceTexture(geode.getStateSet() );
	for ( unsigned int i = 0; i < geode.getNumDrawables(); ++i )
	{
		replaceTexture( geode.getDrawable( i ) -> getStateSet() );
	}
	traverse( geode );
}

// this user method uses getTextureAttribute() to obtain the texture in unit 0 from input state set
// -> replace it with the managed one.
// because the state set is obtained via the getStateSet() method of node or drawable,
// not getOrCreateStateSet() which is sure to return an existing or new state set object,
// the input pointer may be null here:
void replaceTexture( osg::StateSet* ss )
{
	if ( ss )
	{
		osg::Texture* oldTexture = dynamic_cast <osg::Texture*> ( ss -> getTextureAttribute( 0, osg::StateAttribute::TEXTURE ) );
		if ( oldTexture )
		{
			ss -> setTextureAttribute( 0, _texture.get() );
		}
	}
}

int main( int argc, char** argv )
{
	// Load two models as sg's
	// The lz.osg model is used as the main scene
	// and the glider will be treated as a sub-graph that will be rendered to a texture
	// and presented on the surfaces of models in the main scene:
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile("lz.osg");
	osg::ref_ptr <osg::Node> sub_model = osgDB::readNodeFile( "glider.osg" );
	
	// create a new texture object.
	// this differs from the previous example that creates 2D textures
	// and applies an image to it.
	// This time we should specify the texture size,
	// the internal format, and other attributes by ourselves:
	int tex_width = 1024, tex_height = 1024;

	osg::ref_ptr <osg::Texture2D> texture = new osg::Texture2D;
	texture -> setTextureSize( tex_width, tex_height ); 
	texture -> setInternalFormat( GL_RGBA );
	texture -> setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
	texture -> setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );
	
	// use FindTextureVisitor to locate all textures used in the lz.osg model
	// -> replace them with the new, empty texture object:
	FindTextureVisitor ftv( texture.get() );
	if( model.valid() )
	{
		model -> accept( ftv );
	}

	// create the render-to-textures camera
	// we set it up to have the same viewport as the texture size specified, and clrea the background color and buffer when starting to render the sub-scene:
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setViewport( 0, 0, tex_width, tex_height );
	camera->setClearColor( osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f) );
	camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );	
	
	// Force the camera to be rendered before the main scene, and use the high
	// efficiency FBO to implement the render-to-textures technique. The key statement
	// in this example is to bind the color buffer with the texture object, which leads to
	// continuous updates of the texture object, redrawing the sub-scene graph again
	// and again:
	camera->setRenderOrder( osg::Camera::PRE_RENDER );
	camera->setRenderTargetImplementation(
	osg::Camera::FRAME_BUFFER_OBJECT );
	camera->attach( osg::Camera::COLOR_BUFFER, texture.get() );
	
	// Set the camera to be absolute, and set the loaded glider to be its sub-scene graph:
	camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
	camera->addChild( sub_model.get() );

	// Initialize the viewer and set a default manipulator to it:
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );

	// The last step is to animate the glider. We haven't learnt any animation
	// functionalities in OSG, but we already known that the simulation loop can be
	// customized to add some pre- and post-frame events. We will simply modify
	// the view matrix of the render-to-textures camera during each frame, as if
	// making the glider swing. This is done by altering the up direction of the
	// "look-at" view matrix, as shown:
	float delta = 0.1f, bias = 0.0f;
	osg::Vec3 eye(0.0f,-5.0f, 5.0f);
	while ( !viewer.done() )
	{
		if ( bias < -1.0f )
		{	
			delta = 0.1f;
		}
		else if ( bias > 1.0f )
		{	
			delta = -0.1f;
		}
		bias += delta;
		camera->setViewMatrixAsLookAt( eye, osg::Vec3(),
		viewer.frame();
	}
	return 0;
}

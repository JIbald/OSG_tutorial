/*
 * We have learnt how to change the transformation matrix of a model with the
osg::MatrixTransform nodes. With the help of the osg::AnimationPath class and the
osgAnimation namespace, we can even create animation effects on these transformable
objects. But this is not enough for an interactive scene. Our further requirement is to control
scene graph nodes with user input devices. Imagine that we have a submarine, a tank, or a
familiar Cessna in a modern warfare game. It will be really exciting if we can simulate driving
it with the keyboard, mouse, or even joysticks.
 */

#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

/*
 * Our task is to take control of a Cessna model with some keys. To handle these user
events, we have to declare a ModelController class, which is derived from the
osgGA::GUIEventHandler base class, and override the handle() method to
make sure that all user events are passed in as an osgGA::GUIEventAdapter
object. The model pointer is also included in the handler class; otherwise
there is no way to tell which model is going to be controlled:
 */
class ModelController : public osgGA::GUIEventHandler
{
protected:
	osg::ref_ptr <osg::MatrixTransform> _model;

public:
	ModelController( osg::MatrixTransform* node )
		: _model( node )
		{}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
};

/*
 * In the implementation of the handle() method, we will modify the Euler
angles of the member variable _model , which can be a transformation node
representing a Cessna or other models. The character keys w , s , a , and d can
easily describe the heading and pitch rotations of the aircraft via a common
KEYDOWN event. Of course, function keys and navigation keys, including
KEY_Left , KEY_Right , and so on, are also available for use here:
 */

bool ModelController::handle( const osgGA:: GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if ( !_model )
	{
		return false;
	}

	osg::Matrix matrix = _model -> getMatrix();

	switch ( ea.getEventType() )
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
			switch ( ea.getKey() )
			{
				case 'a':
				case 'A':
					matrix *= osg::Matrix::rotate( -0.1f, osg::Z_AXIS );
					break;
				case 'd':
				case 'D':
					matrix *= osg::Matrix::rotate( 0.1f, osg::Z_AXIS );
					break;
				case 'w':
				case 'W':
					matrix *= osg::Matrix::rotate( -0.1f, osg::X_AXIS );
					break;
				case 's':
				case 'S':
					matrix *= osg::Matrix::rotate( 0.1f, osg::X_AXIS );
					break;
				default:
					break;
			}
			_model -> setMatrix( matrix );
			break;
		default:
			break;
	}
	return false;
}

/*
 * In the main function, we will first load the Cessna model and add it to an
osg::MatrixTransform parent. The parent node will be used as the
controlled object and transferred to a ModelController handler instance:
 */
int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );

	osg::ref_ptr <osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt -> addChild( model.get() );

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root -> addChild( mt.get() );
	
	// initialize model controller and ass transformation node as arg
	osg::ref_ptr <ModelController> ctrler = new ModelController( mt.get() );

	/*
	 * We don't want the camera manipulator to work in this example, because it may also
	 * affect the model-view matrix of the viewer when using the keyboard and mouse,
	 * and confuse the result of handling GUI events. Therefore, in addition to adding
	 * the created event handler, we will prevent the main camera from receiving any
	 * user events with the setAllowEventFocus() method, and set a suitable view
	 * matrix by ourselves (because the manipulator can't contact the camera now):
	 */ 
	osgViewer::Viewer viewer;
	viewer.addEventHandler( ctrler.get() );
	viewer.getCamera() -> setViewMatrixAsLookAt( osg::Vec3( 0.0f, -100.0f, 0.0f ), 
						     osg::Vec3(), osg::Z_AXIS );
	viewer.getCamera() -> setAllowEventFocus( false );

	viewer.setSceneData( root.get() );
	return viewer.run();
}





























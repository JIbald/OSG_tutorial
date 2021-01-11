/*
 * Our task this time is to implement a very common task in 3D software—clicking to select
an object in the world and showing a selection box around the object. The bounding
box of the selected geometry should be good for representing a selection box, and the
osg::ShapeDrawable class can quickly generate a simple box for display purposes.
The osg::PolygonMode attribute will then make the rendering pipeline only draw the
wireframes of the box, which helps to show the selection box as brackets. These are all we
need to produce practical picking object functionalities.
 */

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/StateAttribute>
#include <osg/StateSet>
#include <osgDB/ReadFile>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>

/*
 * The PickHandler will do everything required for our task, including an
intersection test of the mouse cursor and the scene graph, creating and returning
the selection box node (the _selectionBox variable in this example), and
transforming the box around the selected object when pressing the mouse button:
 */
class PickHandler : public osgGA::GUIEventHandler
{
public:
	osg::Node* getOrCreateSelectionBox();
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	osg::ref_ptr <osg::MatrixTransform> _selectionBox;
};

/*
 * In the following method, we will allocate and return a valid selection box
node to the main function. There are several points to note here: firstly, the
osg::Box object will not be changed at runtime, but a parent transformation
node will be used and modified instead, for the reason of simplifying
operations; secondly, the GL_LIGHTING mode and the osg::PolygonMode
attribute should be used to make the selection box more natural; finally, there
is also a confusing setNodeMask() call, which will be explained later:
 */
osg::Node* PickHandler::getOrCreateSelectionBox()
{
	if ( !_selectionBox )
	{
		osg::ref_ptr <osg::Geode> geode = new osg::Geode;
		geode -> addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(), 1.0f)) );

		_selectionBox = new osg::MatrixTransform;
		_selectionBox -> setNodeMask ( 0x1 );
		_selectionBox -> addChild( geode.get() );

		osg::StateSet* ss = _selectionBox -> getOrCreateStateSet();
		ss -> setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		ss -> setAttributeAndModes( new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, 								     osg::PolygonMode::LINE) );
	}
	return _selectionBox.get();
}

/*
 *We are going to strictly limit the occasion of picking scene objects to make
sure camera manipulation operations can work. It will only be called when
the user is holding the Ctrl key and releasing the left mouse button. After
that, we obtain the viewer by converting the osgGA::GUIActionAdapter
object, and create an intersection visitor to find a node that can possibly be
picked by the mouse cursor (be aware of the setTraversalMask() method
here, which will be introduced along with the setNodeMask() method).
Then the resulting drawable object and its parent node path can be used
to describe the world position and scale of the bounding selection box:
 */

bool PickHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if ( ea.getEventType() != osgGA::GUIEventAdapter::RELEASE 
				  || ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON 
				  || !( ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL ) )
	{
		return false;
	}

	osgViewer::Viewer* viewer = dynamic_cast <osgViewer::Viewer*> (&aa);
	if( viewer )
	{
		osg::ref_ptr <osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::WINDOW, ea.getX(), ea.getY() );
		osgUtil::IntersectionVisitor iv( intersector.get() );
		iv.setTraversalMask( ~0x1 );
		viewer -> getCamera() -> accept( iv );

		if ( intersector -> containsIntersections() )
		{
			osgUtil::LineSegmentIntersector::Intersection& result = (*intersector -> getIntersections().begin() );

			osg::BoundingBox bb = result.drawable -> getBoundingBox();
			osg::Vec3 worldCenter = bb.center() *osg::computeLocalToWorld( result.nodePath );
			_selectionBox -> setMatrix(
							osg::Matrix::scale( bb.xMax() - bb.xMin(),
									    bb.yMax() - bb.yMin(),
									    bb.zMax() - bb.zMin() ) * osg::Matrix::translate( worldCenter ) );
		}
	}
	return false;
}

/*
 * The remaining work is not hard to understand. We will first construct the scene
graph by adding two models to the root node:
 */
int main( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model1 = osgDB::readNodeFile( "cesssna.osg" );
	osg::ref_ptr <osg::Node> model2 = osgDB::readNodeFile( "cow.osg" );

	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( model1.get() );
	root -> addChild( model2.get() );

	/*
	 * We create the picking handler, and add the value of getOrCreateSelectionBox() 
	 * to the root node, too. This will make the selection box visible in the scene graph:
	 */
	osg::ref_ptr <PickHandler> picker = new PickHandler;
	root -> addChild( picker -> getOrCreateSelectionBox() );
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	viewer.addEventHandler( picker.get() );
	return viewer.run();
}































// see notes for description
#include <osg/View>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgViewer/CompositeViewer>

// function to create osgViewer::View object
// apply an existing node to it
// setUpViewInWindow() produces non-full screen views
osgViewer::View* createView( int x, int y, int w, int h, osg::Node* scene )
{
	osg::ref_ptr <osgViewer::View> view = new osgViewer::View;
	view -> setSceneData( scene );
	view -> setUpViewInWindow( x, y, w, h );
	return view.release();
}

int main ( int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile( "cessna.osg" );
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile( "cow.osg" );
	osg::ref_ptr<osg::Node> model3 = osgDB::readNodeFile( "glider.osg" );
	
	// three views within small 320 x 240 windows at specific position
	osgViewer::View* view1 = createView(50, 50, 320, 240, model1);
	osgViewer::View* view2 = createView(370, 50, 320, 240, model2);
	osgViewer::View* view3 = createView(185, 310, 320, 240, model3);
			
	// add views to composite viewer and start simulation as if single viewer
	// while loop also usable in this case.
	osgViewer::CompositeViewer viewer;
	viewer.addView(view1);
	viewer.addView(view2);
	viewer.addView(view3);
	return viewer.run();
}

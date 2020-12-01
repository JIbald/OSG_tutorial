#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main ( int argc, char** argv )
{
	//load two models and assign them to Node pointers
	osg::ref_ptr <osg::Node> model1 = osgDB::readNodeFile ( "cessna.osg" );
	osg::ref_ptr <osg::Node> model2 = osgDB::readNodeFile ( "cow.osg" );

	//add two models to Group node by using addChild()
	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( model1.get() );
	root -> addChild( model2.get() );

	//Initialize and start the viewer
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

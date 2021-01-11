// do you remember that we have designed an animated switch node in Chapter 5?
// It is derived form osg::Switch, but will automatically change the states of its first two children
// according to an internal counter, through overriding the traverse() virtual method.
//
// Now we would like to redo the same task, 
// but this time using the UPDATE CALLBACK mechanism.
// This requires customizing a new class derived from the osg::NodeCallback base class,
// and overriding the operator() to perform the execution in the callback implementation

#include <osg/Node>
#include <osg/Switch>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

// declare the SwitchingCallback class.
// it is an osg::NodeCallback based class, which can soon be used as update, event,
// or cull callbacks of scene nodes.
// The only important virtual method to implement is operator().
// This is automatically called during the update, event, or cull traversal of the scene graph.
// Besides, we also initialize the member variable _count, as an internal counter:
class SwitchingCallback : public osg::NodeCallback
{
public:
	SwitchingCallback() : _count(0) {}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

protected:
	unsigned int _count;
};

// the operator() has two input params:
// 	-> node associated w/ the callback
// 	-> node visitor calling the function during traversals.
// to animate the state switching of the two child nodes,
// we have to convert the node pointer to the type osg::Switch.
// A static_cast<> is used here because we are sure that the associated node is a switch node.
// 
// NOTE:	traverse(0 method should be executed in a certain location,
// 		to ensure that the update traversal visitor can continure traversing the scene graph.
void SwitchingCallback::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	osg::Switch* switchNode = static_cast <osg::Switch*>( node );
	if ( ! ( (++_count) %60 ) && switchNode )
	{
		switchNode -> setValue( 0, !switchNode -> getValue(0) );
		switchNode -> setValue( 1, !switchNode -> getValue(1) );
	}
	traverse( node, nv );
}



// The next step was already introduced in ch5: managing scene graph:
// Load two models that show two different states of a cessna,
// put them under the switch node
// which will be used in the customized update callback SwitchingCallback:
int main( int argc, char** argv )
{
osg::ref_ptr <osg::Node> model1 = osgDB::readNodeFile( "cessna.osg" );
osg::ref_ptr <osg::Node> model2 = osgDB::readNodeFile( "cessnafire.osg" );
osg::ref_ptr <osg::Switch> root = new osg::Switch;
root -> addChild( model1.get(), false );
root -> addChild( model2.get(), true );

// attach update callback object to the node.
// If you are tired of executing this callback in every frame, just retransfer a NULL argument to the setUpdateCallback() method.
// 	-> callback obj will be deleted if its referenced count is down to 0:
root -> setUpdateCallback( new SwitchingCallback );

osgViewer::Viewer viewer;
viewer.setSceneData( root.get() );
return viewer.run();

}
































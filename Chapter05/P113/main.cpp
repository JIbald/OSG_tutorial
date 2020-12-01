//osg:Switch class can display specified child nodes while hiding others.
// - used to represent the animations states of various objects (traffic lights)
//however a typical osg::Switch node can't automatically switch between children at different times.
//based on that, we will develop a new "Animation Switch" node, 
//which displays it's children at one time
//and reverse the switch state to a user-defined internal counter

#include <osg/Switch>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgUtil/CullVisitor>

// class AnimatingSwitch
//derived from osg::Switch to use setValue() method
//macro META_Node used to define basic properties (library and class name) of node

class AnimatingSwitch :public osg::Switch
{
public:
	AnimatingSwitch()
		: osg::Switch(), _count( 0 ) 
	{}

	AnimatingSwitch( const AnimatingSwitch& copy,
			 const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY
		       )
		: osg::Switch( copy, copyop ), _count( copy._count)
	{}

	META_Node( osg, AnimatingSwitch );

	virtual void traverse( osg::NodeVisitor& nv );

protected:
	unsigned int _count;
};

// traverse() 
// increase the internal counter and see if it reaches a multiple of 60
// and reverse the states of the first and second child nodes

void AnimatingSwitch::traverse( osg::NodeVisitor& nv )
{	
	osgUtil::CullVisitor* cv = dynamic_cast <osgUtil::CullVisitor*> ( &nv );
	if( cv )
	{

		if( !( ( ++_count ) ) % 14400 )
		{
			setValue( 0, !getValue(0) );
			setValue( 1, !getValue(1) );
		}
	}
	osg::Switch::traverse( nv );
}

int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr <osg::Node> model2 = osgDB::readNodeFile("cessnafire.osg");
	
	osg::ref_ptr <AnimatingSwitch> root = new AnimatingSwitch;
	root -> addChild( model1.get(), true );
	root -> addChild( model2.get(), false );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}	

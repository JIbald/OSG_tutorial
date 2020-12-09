#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <iostream>

//InfoVisitor class
//define necessary virtual methods
//we only handle leaf nodes and common osg::Node objects.
//inline function spaces() is used for printing spaces before node information to indicate level in tree


class InfoVisitor : public osg::NodeVisitor
{
public:
	InfoVisitor() : _level( 0 )
	{
		setTraversalMode( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	}

	std::string spaces()
	{
		return std::string( _level * 2, ' ' );
	}

	virtual void apply( osg::Node& node );
	virtual void apply( osg::Geode& geode );

protected:
	unsigned int _level;
};

//className() and libraryName(), both return const char* values
//for instance: "Node" as the class name and "osg" as the library name. 
//there is no trick in re-implementing these two methods for different classes. 
//META_Object and META_Node macro will do work internally:

void InfoVisitor::apply( osg::Node& node )
{
	std::cout << spaces() << node.libraryName() << "::"
		  << node.className() << std::endl;

	_level++;
	traverse( node );
	_level--;
}

//implementation of apply() overloaded method with osg::Geode& param
//is slightly different from the previous one.
//iterates all attached drawables of the osg::Geode node and prints their information, too.
//Be aware of the calling time of traverse() here,
//which ensures that the level of each node in the tree is correct.

void InfoVisitor::apply( osg::Geode& geode ) //InfoVisitor:: might be missing in tutorial pdf
{
	std::cout << spaces() << geode.libraryName() << "::"
		  << geode.className() << std::endl;

	_level++;

	for ( unsigned int i = 0; i < geode.getNumDrawables(); ++i )
	{
		osg::Drawable* drawable = geode.getDrawable( i );
		std::cout << spaces() << drawable -> libraryName() << "::"
			  << drawable -> className() << std::endl;
	}

	traverse( geode );
	_level--;
}

int main ( int argc, char** argv )
{
	osg::ArgumentParser arguments( &argc, argv );
	osg::ref_ptr <osg::Node> root = osgDB::readNodeFiles( arguments );

	if( !root )
	{
		OSG_FATAL << arguments.getApplicationName() << ": No data loaded." << std::endl;
		return -1;
	}

	//use InfoVisitor to visit the loaded model now
	//notice that setTraversalMode*( is called in the constructor of the visitor
	//in order to enable the traversal of all its children
	InfoVisitor infoVisitor;
	root -> accept( infoVisitor );
}

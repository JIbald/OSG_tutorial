#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <fstream>

#include "MonitoringTarget.h"
#include "LogFileHandler.h"

int main( int argc, char** argv )
{
	//osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessna.osg" ); 
	// root variable, provides rt access to cessna.osg
	
	//osgViewer::Viewer viewer;
	//viewer.setSceneData( root.get() );
	//return viewer.run();

	//------------------
	//time for action monitoring counted objects && have a go hero - returning from a function
	
	//osg::ref_ptr <MonitoringTarget> target;
        //target->MonitoringTarget::createMonitoringTarget(0);

	//std::cout << "Referenced count before referring: "
		  //<< target -> referenceCount() << std::endl;

	//osg::ref_ptr<MonitoringTarget> anotherTarget = target;
	//std::cout << "Referenced count after referring: "
		  //<< target -> referenceCount() << std::endl;

	//for ( unsigned int i = 1; i < 5; ++i )
	//{
		//osg::ref_ptr<MonitoringTarget> subTarget;
		//subTarget->MonitoringTarget::createMonitoringTarget(i);
	//}
	
	//-------------------
	//time for action - reading the model filename from the command line
	
	//osg::ArgumentParser arguments( &argc, argv );
	//std::string filename;
	//arguments.read( "--model", filename );

	//osg::ref_ptr<osg::Node> root = osgDB::readNodeFile( filename );
	//osgViewer::Viewer viewer;
	//viewer.setSceneData( root.get() );
	//return viewer.run();

	//------------------------------------------------------------------------
	//p.58 saving the log file
	osg::setNotifyLevel( osg::INFO );
	osg::setNotifyHandler( new LogFileHandler( "output.txt" ) );

	osg::ArgumentParser arguments( &argc, argv );
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFiles( arguments );

	if( !root )
	{
		OSG_FATAL << arguments.getApplicationName() << ": Nod data loaded." << std::endl;
		return -1;
	}

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();

}

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <fstream>

class LogFileHandler : public osg::NotifyHandler
{
public:
	LogFileHandler( const std::string& file )
	{
		_log.open( file.c_str() );
	}

	virtual ~LogFileHandler()
	{
		_log.close();
	}

	virtual void notify( osg::NotifySeverity severity, const char* msg )
	{
		_log << msg;
	}

protected:
	std::ofstream _log; 
};

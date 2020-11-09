#include <osg/ref_ptr>
#include <osg/Referenced>
#include <iostream>

class MonitoringTarget : public osg::Referenced
{
public:
	MonitoringTarget( int id ) : _id(id)
	{
		std::cout << "Constructing target: " << _id << std::endl;
	}

	MonitoringTarget* createMonitoringTarget( unsigned int id )
	{
		osg::ref_ptr<MonitoringTarget> target = new MonitoringTarget(id);
		return target.release();
	}

protected:
	virtual ~MonitoringTarget()
	{
		std::cout << "Destroying target: " << _id << std::endl;
	}

	int _id;
};

/*
 * In this example, we will design a new thread with the OpenThreads library and use it to read
characters from the standard input. At the same time, the main process, that is, the OSG
viewer and rendering backend will try retrieving the input characters and displaying them on
the screen with the osgText library. The entire program can only quit normally when the data
thread and main process are both completed.
 */

#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <iostream>

/*
 * Declare our new DataReceiverThread class as being derived from
OpenThreads::Thread . Two virtual methods should be implemented to ensure
that the thread can work properly: the cancel() method defines the cancelling
process of the thread, and the run() method defines what action happens from
the beginning to the end of the thread. We also define a mutex variable for inter-
process synchronization, and make use of the singleton pattern for convenience:
 */

class DataReceiverThread : public OpenThreads::Thread
{
public:
	static DataReceiverThread* instance()
	{
		static DataReceiverThread s_thread;
		return &s_thread;
	}


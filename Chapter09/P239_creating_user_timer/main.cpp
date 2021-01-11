/*
 * A timer event is emitted whenever the internal counter reaches a specified interval. This
is common in various GUI systems, and allows users to set a customized timer callback to
receive the timing message and implement related operations.
Now we are able to realize the same work in OSG. As there are no standard timer events
defined in the osgGA::GUIEventAdapter class, we have to make use of the USER event
type, as well as an additional data pointer.
 */

#include <osg/Switch>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/Viewer>
#include <iostream>

/*
 * A TimerInfo structure is first defined to manage the parameters (mainly
the trigger time of the timer) of the timer event. We have to attach this
osg::Referenced derived pointer to the userEvent() method, because it
is the only element for distinguishing between different customized events:
 */
struct TimerInfo : public osg::Referenced
{
	TimerInfo( unsigned int c ) 
		: _count( c ) {}

	unsigned int _count;
};

/*
 * The TimerHandler is used for both processing the timer object and handling
timer events. We would like to switch between normal and afire states of a
Cessna model every time we receive such an event. In Chapter 5, Managing
Scene Graph and Chapter 8, Animating Scene Object, this is done by customizing
the osg::Node class and the update callback. But this time we will try using
the event handler with an input osg::Switch pointer as the argument:
 */
class TimerHandler : public osgGA::GUIEventHandler
{
protected:
	osg::ref_ptr <osg::Switch> _switch;
	unsigned int _count;

public:
	TimerHandler( osg::Switch* sw ) 
		: _switch( sw ), _count( 0 ) {}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
};

/*
 * There are two kinds of events to handle in the overrode handle() method.
The FRAME event is automatically emitted with every frame and can be
used to manage and increase an internal counter, and send userEvent()
to the event queue when time is ripe. In this example, we assume that the
timer event is emitted every 100 counts. The other one is the USER event,
which doesn't contain any information except a TimerInfo object as the
"user data" to indicate the timer and its counts. Here, we will print the count
number and switch between child nodes of the variable _switch :
 */
bool TimerHandler::handle( const osgGA::GUIEventAdapter& ea, 
			   osgGA::GUIActionAdapter& aa )
{
	switch ( ea.getEventType() )
	{
		switch ( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::FRAME :
				if ( _count % 100 == 0 )
				{
					osgViewer::Viewer* viewer = 										dynamic_cast <osgViewer::Viewer*> ( &aa );
					if ( viewer )
					{
						viewer -> getEventQueue() 										-> userEvent( new TimerInfo( _count ) );
					}
				}
				_count++;
				break;

			case osgGA::GUIEventAdapter::USER :
				   if ( _switch.valid() )
				   {
					   const TimerInfo* ti = 
						   dynamic_cast <const TimerInfo*> (ea.getUserData() );
					   std::cout << "Timer event at: " << ti -> _count << std::endl;

					   _switch -> setValue( 0, !_switch -> getValue( 0 ) );
					   _switch -> setValue( 1, !_switch -> getValue( 1 ) );
				   }
				   break;
			default:
				   break;
		}
		return false;

	}
}


/*
 * In the main function, we simply create the switch node, which contains a normal
Cessna model and an afire one:
 */

int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model1 = osgDB::readNodeFile( "cessna.osg" );
	osg::ref_ptr <osg::Node> model2 = osgDB::readNodeFile( "cessnafire.osg" );

	osg::ref_ptr <osg::Switch> root = new osg::Switch;
	root -> addChild( model1.get(), false );
	root -> addChild( model2.get(), true );

	// add the timer event sender and handler to the viewer and start it:
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	viewer.addEventHandler( new TimerHandler( root.get() ) );
	return viewer.run();
}
























































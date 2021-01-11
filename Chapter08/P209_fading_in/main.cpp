/*
 * We already had experience of making a scene object translucent using osg::BlendFunc
class and rendering orders. The fourth component of the color vector, called the alpha value,
will do the trick for us. But what will happen if we have a continuously-changing alpha? The
object will be completely transparent (invisible) when alpha is 0, and completely opaque
when it is 1.0. The animating process from 0.0 to 1.0 will therefore cause the object to
gradually appear to viewers, that is, the fade-in effect.

The update callback can be used in this task. It is no problem to create an
osg::NodeCallback based class and set it to the node that will be fading in. But the
state attribute callback, osg::StateAttributeCallback , is also available in this case.

The osg::Material class is used here to provide the alpha bit of each geometry vertex,
instead of just setting a color array.

 */

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osgAnimation/EaseMotion>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

/*
 *To instantiate an osg::StateAttributeCallback , we have to override the
operator() method and make use of its arguments: the state attribute itself
and the visitor who is traversing it. An additional work item here is to declare an
ease motion interpolator using a cubic function at the in and out position of the
animation curve:
 */

class AlphaFadingCallback : public osg::StateAttributeCallback
{
protected:
	osg::ref_ptr<osgAnimation::InOutCubicMotion> _motion;

public:
	AlphaFadingCallback()
	{ 
		_motion = new osgAnimation::InOutCubicMotion(0.0f, 1.0f); 
	}

	virtual void operator()(osg::StateAttribute*,osg::NodeVisitor*);

};

/*
 *In the operator() , we will obtain the material attribute of the scene object, which
can be used for simulating transparent and translucent effects. There are two steps
to follow: firstly, update the ease motion object with a customized delta time value;
after that, retrieve the result of motion between 0 and 1, and apply it to the alpha
component of the material's diffuse color:
 */








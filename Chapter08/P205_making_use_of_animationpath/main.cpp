/*
 *Now we are going to make our Cessna navigate a cruise. It will be moving in a circle with
the centre at (0, 0, 0). The animation path is used for updating the position and orientation
of the model continuously, using linear interpolation between key-frames. The only work
is to add control points, including position, optional rotation, and scale key value, to the
animation timeline.
 */

#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

/*
 * Create the animation path. This is in fact a circle with a specific radius on the XOY
plane. The time parameter is used to indicate the total time required to finish a lap.
The osg::AnimationPath object is set to loop the animation infinitely. It contains
32 control points to form a circle path, which is determined by the local variable
numSamples :
 */
osg::AnimationPath* createAnimationPath( float radius, float time )
{
	osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
	path->setLoopMode( osg::AnimationPath::LOOP );
	unsigned int numSamples = 32;
	float delta_yaw = 2.0f * osg::PI / ((float)numSamples - 1.0f);
	float delta_time = time / (float)numSamples;

	for ( unsigned int i=0; i<numSamples; ++i )
	{
		float yaw = delta_yaw * (float)i;
		osg::Vec3 pos( sinf(yaw)*radius, cosf(yaw)*radius, 0.0f );
		osg::Quat rot( -yaw, osg::Z_AXIS );
		path->insert( delta_time * (float)i, osg::AnimationPath::ControlPoint(pos, rot) );
	}
	return path.release();
}

/*
 * Load the Cessna model. You will have noticed that there is a significant difference
between this and previous file names. The string "0,0,90.rot" seems redundant
and ambiguous here. It is a kind of pseudo-loader, which is written as part of
filename but actually does a 90 degrees rotation around the Z axis to the model
cessna.osg . We are going to discuss this in detail in Chapter 10, Creating
Components and Extending Functionality:
 */

int main ( int argc, char** argv )
{
	osg::ref_ptr<osg::Node> model =
	osgDB::readNodeFile( "cessna.osg.0,0,90.rot" );
	osg::ref_ptr<osg::MatrixTransform> root = new
	osg::MatrixTransform;
	root->addChild( model.get() );

	/*
	 * Add the animation path to the osg::AnimationPathCallback object, 
	 * and attach the callback to a node. Note that the animation path can only affect 
	 * osg::MatrixTransform and osg::PositionAttitudeTransform nodes, 
	 * updating their transformation matrices or position and rotation attributes in the 
	 * update traversal:
	 */
	osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
	apcb->setAnimationPath( createAnimationPath(50.0f, 6.0f) );
	root->setUpdateCallback( apcb.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}









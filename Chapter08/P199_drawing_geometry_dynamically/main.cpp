/*
 * It is common practice to modify geometries' vertices and primitive attributes dynamically.
We can change the position, normal, color, and texture coordinate of each vertex, as well as
related primitives per frame, in order to implement kinds of morph animations. During the
modifications, it is important to keep an eye on the data variance, because the draw traversal
might be running while updating vertices and primitives, which might cause conflicts and
even crashes.
In this example, we will make use of the quad geometry that was created in Chapter 4,
Building Geometry Models. We will simply alter its last vertex and make it rotate around
the X axis, which results in a simple animation effect.
*/

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/NodeVisitor>
#include <osgViewer/Viewer>

/*
 *
 * The creation of a quad is familiar to us. Specify the vertex, normal, and color array,
and add a primitive set to indicate that all vertices are arranged and rendered with
the type of GL_QUADS . Finally, return the newly-allocated geometry object:
 *
 */

osg::Geometry* createQuad()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f) );
	vertices->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
	vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
	colors->push_back( osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) );
	colors->push_back( osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
	colors->push_back( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray( vertices.get() );
	quad->setNormalArray( normals.get() );
	quad->setNormalBinding( osg::Geometry::BIND_OVERALL );
	quad->setColorArray( colors.get() );
	quad->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	quad->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

	return quad.release();
}

/*
 *With the help of osg::Drawable::UpdateCallback , we can easily obtain
the geometry pointer for altering each frame. The only method to be overridden
is update() , which has a node visitor and a drawable pointer parameter. Its
super class, osg::Drawable::UpdateCallback , is a little similar to the
osg::NodeCallback class, except that a drawable's callback doesn't have to
traverse to any "child" (a drawable has no child).
 */
class DynamicQuadCallback : public osg::Drawable::UpdateCallback
{
public:
	virtual void update( osg::NodeVisitor*, osg::Drawable* drawable );
};

/*
 * In the implementation of the update() method, we read out the vertex
array of the created quad geometry with two static_cast<> operators. The
dynamic_cast<> keyword might be safer here if the DynamicQuadCallback
class is not only applied to osg::Geometry , but is also applied to other customized
drawables. After that, we quickly rotate the last vertex in the array around the origin
(0, 0, 0), using the osg::Quat quaternion class. The last work before exiting the
method is to recalculate the display list object and bounding box of the current
geometry, which may need to be updated when any of the vertices are changed:
 */
void DynamicQuadCallback::update( osg::NodeVisitor*, osg::Drawable* drawable )
{
	osg::Geometry* quad = static_cast <osg::Geometry*>( drawable );
	if ( !quad )
	{
		return;
	}

	osg::Vec3Array* vertices = static_cast <osg::Vec3Array*> ( quad -> getVertexArray() );
	if ( !vertices )
	{
		return;
	}

	osg::Quat quat( osg::PI * 0.01, osg::X_AXIS);
	vertices -> back() = quat * vertices -> back();

	quad -> dirtyDisplayList();
	quad -> dirtyBound();
}

/*
 * We define the geometry as DYNAMIC , so the drawing traversal of the OSG backend
will automatically order the dynamic object to perform robust scene graph
traversals. In addition, the drawable's modification callback is specified by the
setUpdateCallback() method of the osg::Drawable class:
 */
int main( int argc, char** argv)
{
	osg::Geometry* quad = createQuad();
	quad -> setDataVariance( osg::Object::DYNAMIC );
	quad -> setUpdateCallback( new DynamicQuadCallback );

	/*
	 * Now, add the quad geometry to an osg::Geode node, 
	 * and attach the root node to the viewer:
	 */
	osg::ref_ptr <osg::Geode> root = new osg::Geode;
	root -> addDrawable( quad );
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}






















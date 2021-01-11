/*
 * The prerequisite for implementing billboards in OSG is to create one or more quad
geometries first. These quads are then managed by the osg::BillBoard class. This forces
all child drawables to automatically rotate around a specified axis, or face the viewer. These
can be done by presetting a unified normal value and rotating each billboard according to
the normal and current rotation axis or viewing vector.
[ 292 ]Chapter 11
We will create two banks of OSG banners, arranged in a V, to demonstrate the use of
billboards in OSG. No matter where the viewer is and how he manipulates the scene camera,
the front faces of banners are facing the viewer all the time. This feature can then be used to
represent textured trees and particles in user applications.
 */

#include <osg/Billboard>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

/*
 * Create the quad geometry directly from the osg::createTexturedQuadGe
ometry() function. Every generated quad is of the same size and origin point,
and uses the same image file. Note that the osg256.png file can be found in
the data directory of your OSG installation path, but it requires the osgdb_png
plugin for reading image data. Please refer to Chapter 10, Saving and Loading
Files if you have any problems in configuring and compiling this plugin.
 */

osg::Geometry* createQuad()
{
	osg::ref_ptr <osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr <osg::Image> image = osgDB::readImageFile( "Images/osg256.png" );
	texture -> setImage( image.get() );

	osg::ref_ptr <osg::Geometry> quad = osg::createTexturedQuadGeometry(
							osg::Vec3( -0.5f, 0.0f, -0.5f ),
							osg::Vec3( 1.0f, 0.0f, 0.0f ),
							osg::Vec3( 0.0f, 0.0f, 1.0f ) );

	osg::StateSet* ss = quad -> getOrCreateStateSet();
	ss -> setTextureAttributeAndModes( 0, texture.get() );
	return quad.release();
}

/*
 * In the main entry, we first create the billboard node and set the mode to
POINT_ROT_EYE . That is, the drawable will rotate to face the viewer and keep
its Z axis upright in the rendering window. The default normal setting of the
osg::BillBoard class is the negative Y axis, so rotating it to the viewing
vector will show the quads on the XOZ plane in the best appearance:
 */
int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Billboard> geode = new osg::Billboard;
	geode -> setMode( osg::Billboard::POINT_ROT_WORLD ); //POINT_ROT_EYE, POINT_ROT_WORLD, AXIAL_ROT

	// banner quads in v formation
	osg::Geometry* quad = createQuad();
	for ( unsigned int i = 0; i < 10; ++i )
	{
		float id = (float)i;
		geode -> addDrawable( quad, osg::Vec3( -2.5f + 0.2f * id, id, 0.0f ) );
		geode -> addDrawable( quad, osg::Vec3( 2.5f - 0.2f * id, id, 0.0f ) );
	}

	/*
	 * All quad textures' backgrounds are automatically cleared because of the alpha
	 * test, which is performed internally in the osgdb_png plugin. That means we have to
	 * set correct rendering order of all the drawables to ensure that the entire process
	 * is working properly:
	 */
	osg::StateSet* ss = geode -> getOrCreateStateSet();
	ss -> setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	osgViewer::Viewer viewer;
	viewer.setSceneData( geode.get() );
	return viewer.run();
}














































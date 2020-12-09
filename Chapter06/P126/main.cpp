//polygon rasterization mode of a loaded model
//osg::Polygonmode class is derived form osg::StateATrribute base class
//it encapsulates OpenGL's glPolygonMode() function
//implements interfaces for specifying face and drawing mode params
// -> changes final rasterization of the attached node


#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

//two osg::MatrixTransform nodes sharing same model
//placed at different positions 

int main ( int argc, char** argv )
{
	osg::ref_ptr <osg::Node> model = osgDB::readNodeFile( "cessna.osg" );

	osg::ref_ptr <osg::MatrixTransform> transformation1 = new osg::MatrixTransform;
	transformation1 -> setMatrix( osg::Matrix::translate( -25.0f, 0.0f, 0.0f ) );
	transformation1 -> addChild( model.get() );

	osg::ref_ptr <osg::MatrixTransform> transformation2 = new osg::MatrixTransform;
	transformation2 -> setMatrix( osg::Matrix::translate( 25.0f, 0.0f, 0.0f ) );
	transformation2 -> addChild( model.get() );

	//add osg::PolygonMode attribute to stateset of transformation1
	//using setMode() with 2 params: 
	//	-the face the mode is applied to
	//	-mode in which polygons will be rasterized
	osg::ref_ptr <osg::PolygonMode> pm = new osg::PolygonMode;
	pm -> setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	transformation1 -> getOrCreateStateSet() -> setAttribute( pm.get() );

	//add nodes to root node
	osg::ref_ptr <osg::Group> root = new osg::Group;
	root -> addChild( transformation1.get() );
	root -> addChild( transformation2.get() );

	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

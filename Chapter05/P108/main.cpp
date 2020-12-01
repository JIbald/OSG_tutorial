#include <osg/LOD>
#include <osgDB/ReadFile>
#include <osgUtil/Simplifier>
#include <osgViewer/Viewer>

//create "discrete LOD node" with set of predefined objectr to represent the same model.
//these objects are used as child nodes of the osg::LOD node and sidplayed at different distances.
//We will use the internal polygon reduction technique class osgUtil::Simplifier
//to generate various detailed objects from an original model.

int main ( int argc, char** argv )
{
	//we build three levels of model details.
	//we create three copies of the original model. 
	//we read the cessna from the file three times which is ok,
	//but we call clone() to duplicate the loaded model for immediate uses:
	osg::ref_ptr <osg::Node> modelL3 = osgDB::readNodeFile( "cessna.osg" );
	osg::ref_ptr <osg::Node> modelL2 = dynamic_cast <osg::Node*> ( modelL3 -> clone( osg::CopyOp::DEEP_COPY_ALL ) );
	osg::ref_ptr <osg::Node> modelL1 = dynamic_cast <osg::Node*> ( modelL3 -> clone( osg::CopyOp::DEEP_COPY_ALL ) );

	//lvl 3 will be original cessna with max num of polygons
	//lvl 2 has fewer polygons 
	//lvl 1 is least detailed only displayed at very far distance
	//osgUtil::Simplifier class is used here to reduce the certices and faces.
	//setSampleRatio() to lvl 1 and lvl 2 model with different values which results in different simplifying rates:
	osgUtil::Simplifier simplifier;
	simplifier.setSampleRatio( 0.5 );
	modelL2 -> accept( simplifier );
	
	simplifier.setSampleRatio( 0.1 );
	modelL1 -> accept( simplifier );

	//add level models to the LOD node and set their visible range in descending order
	//don't make overlapping ranges when you are configuring min and max range values
	//with addChild() or setRange(), otherwise mor than one lvl of model will be shown at same pos
	//results in incorrect behaviours
	osg::ref_ptr <osg::LOD> root = new osg::LOD;
	root -> addChild( modelL1.get(), 200.0f, FLT_MAX );
	root -> addChild( modelL2.get(), 50.0f, 200.0f );
	root -> addChild( modelL3.get(), 0.0f, 50.f);

	//app might need more time to compute and reduce model faces
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	return viewer.run();
}

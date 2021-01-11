cmake_minimum_required(VERSION 2.6)

PROJECT(MyProject)

#find_package( Threads::Threads REQUIRED ) #new
find_package( OpenGL )
find_package( GLUT REQUIRED )
find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer )
find_package( osgGA )
find_package( osgText )
find_package( osgParticle )
find_package( osgShadow )
find_package( osgFX )

set( SET_PREFER_PTHREAD_FLAG ON )

macro ( config_project PROJNAME LIBNAME )
		include_directories( ${${LIBNAME}_INCLUDE_DIR} )
		target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARIES} ) #was _LIBRARY
endmacro()

add_executable( MyProject main.cpp )
config_project( MyProject OPENTHREADS )
config_project( MyProject OSG )
config_project( MyProject OSGDB )
config_project( MyProject OSGUTIL )
config_project( MyProject OSGVIEWER )
config_project( MyProject FREEGLUT )
config_project( MyProject GLUT )
config_project( MyProject OPENGL )
config_project( MyProject OSGGA )
config_project( MyProject OSGTEXT )
config_project( MyProject OSGPARTICLE )
config_project( MyProject OSGSHADOW )
config_project( MyProject oSGFX )
#config_project( MyProject Threads::Threads ) #new

/*
		a structure defining a path, to be followed by an object of type NPC

	Written by Tyler Raborn
*/

//C++ headers
#include <deque>

//C headers
#include <cstdio>

//OpenGL Headers
#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//local headers
#include "Vertex.h"
#include "vec3.h"

//objDef
#ifndef _PATH_H
#define _PATH_H

class Path
{

public:
	Path();
	explicit Path(GLfloat[3], GLfloat[3]);
	explicit Path(Vertex, Vertex);
	Path(Vertex*, int); //accepts a STARTPOINT, an ENDPOINT, and an array of size N filled with vertices to be used as a series of waypoints IN BETWEEN the designated start/end points
	~Path();

	bool hasWaypointList();
	Vertex startPoint;
	Vertex endPoint;	
	std::deque<Vertex> waypoints; //holds a list of vertices/points to be traversed *IN BETWEEN* start and end points.

private:

	bool hasWaypoints; //only true if the below deque contains additional waypoints
	
};

#endif
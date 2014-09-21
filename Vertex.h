//written by Tyler Raborn

//C Headers:
#include <cstdio>
#include <cmath>

//OpenGL headers:
#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//local headers:
//#include "vec3.h"

#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{

public:

	//public constructors
	Vertex();
	Vertex(GLdouble, GLdouble, GLdouble);
	Vertex(GLdouble, GLdouble, GLdouble, GLdouble);
	Vertex(GLdouble*);
	Vertex(GLfloat*);
	GLdouble* normal;

	~Vertex();

	//public fields
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble w;

	//public methods:
	void print();
	//GLdouble* toArray(); //converts the vertex to a double array. Usually used for the convenience of my matrix functions.
	GLfloat getDistance(GLfloat[3]);
	GLfloat getDistance(GLdouble*);
	

private:
	//GLdouble* v;

};


#endif
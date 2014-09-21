/*

	written by Charlies Koch and Tyler Raborn

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
#include "vec3.h"

//objdef
#ifndef _LIGHT_H
#define _LIGHT_H

class Light
{
public:
	Light();
	Light(GLdouble[]);
	~Light();

	GLdouble *getPos();
	vec3 *getDir();
	GLdouble *getAmbient();
	GLdouble *getDiffuse();
	GLdouble *getEmission();
	GLdouble getShine();

	//light location:
	GLdouble position[3];
	vec3 dir;

	//light properties:
	GLdouble amb[3];
	GLdouble diff[3];
	GLdouble emission[3];
	GLdouble specular[3];
	GLdouble shine;

private:


};

#endif
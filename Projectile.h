//written by Tyler Raborn
//a projectile that travels in a STRAIGHT path until range exceeded.

//C++ headers
#include <list>

//platform:
#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//locals
#include "Shape.h"
#include "vec3.h"
#include "NPC.h"

#define LASER_SPEED_SLOW 10.0
#define LASER_SPEED_MEDIUM 20.0
#define LASER_SPEED_FAST 30.0	

#define HOSTILE 1
#define FRIENDLY 0

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

extern int playerHit;

class Projectile
{

public:
	Projectile();
	Projectile(GLfloat, vec3*, Shape*, GLfloat, int);
	~Projectile();

	void update();
	void update(std::list<NPC*>*, GLfloat[3], float*, std::list<NPC*>*); //accepts a queue of targets for intersect calculations
	bool outOfRange();
	Shape *getShape();
	vec3 *getVector();
	void align(vec3*);
	int getIFF();
	
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	
	Shape *target;

private:

	Shape *parentShape; //maintains a pointer to the shape object where this projectile originated (if applicable)

	GLfloat vectorX;
	GLfloat vectorY;
	GLfloat vectorZ;

	GLfloat speed;

	GLfloat range;

	//the x/y/z values of the world position where the projectile will die
	GLfloat endPositionX;
	GLfloat endPositionY;
	GLfloat endPositionZ;

	int iff;

};

#endif
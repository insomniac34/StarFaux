
//written by Tyler Raborn

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Shape.h"
#include "Vertex.h"
#include "config.h"

#ifndef _POWER_UP
#define _POWER_UP

class PowerUp
{

public:
	PowerUp();
	PowerUp(Shape*, float, float);
	~PowerUp();

	void update(GLfloat*, float*, float*);
	bool getStatus();

private:

	bool isTaken;
	Shape *targetShape;
	int bounceStatus;

	float boostBoost;
	float healthBoost;
};

#endif 
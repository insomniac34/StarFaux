//written by Tyler Raborn

#include <cstdio>
#include <cmath>
#include "Vertex.h"


#ifndef _VEC_3
#define _VEC_3

class vec3
{

public:
	vec3();
	vec3(GLdouble, GLdouble, GLdouble);
	vec3(GLdouble[3]);
	explicit vec3(Vertex, Vertex); 
	explicit vec3(vec3, GLdouble[4][4]); 
	~vec3();

	GLdouble dot(vec3); //returns the dot product of this vector and the passed-in vector
	vec3* cross(vec3); //returns the cross product of this vector and the passed-in vector
	vec3* normalize(); //returns the normalized variant of this vector
	vec3* multMatrix(GLdouble[4][4]);
	GLdouble* toArray();
	void print();

	GLdouble x;
	GLdouble y;
	GLdouble z;

	GLdouble length;

private:
	GLdouble* vectorMatrixMult(GLdouble[4][4]);
	GLdouble w;



};

#endif
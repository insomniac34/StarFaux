//written by Tyler Raborn

#include "Vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(GLdouble c1, GLdouble c2, GLdouble c3)		
{
	x = c1;
	y = c2;
	z = c3;
	w = 1.0;
}

Vertex::Vertex(GLdouble* v)
{
	GLdouble v0 = v[0];
	GLdouble v1 = v[1];
	GLdouble v2 = v[2];

	x = v0;
	y = v1;
	z = v2;
	w = 1.0;
}

Vertex::Vertex(GLfloat* v)
{
	GLdouble v0 = v[0];
	GLdouble v1 = v[1];
	GLdouble v2 = v[2];

	x = v0;
	y = v1;
	z = v2;
	w = 1.0;
}

void Vertex::print() //for testing
{
	printf("\nVertex: {%g, %g, %g, %g}\n", x, y, z, w);
}

float Vertex::getDistance(GLfloat *v)
{
	Vertex ret(
		       (this->x - v[0]), 
		       (this->y - v[1]), 
		       (this->z - v[2])
               );

	return (float)sqrt(
					   (ret.x * ret.x) +
					   (ret.y * ret.y) +
				       (ret.z * ret.z)
					  );
}

float Vertex::getDistance(GLdouble *v)
{
	Vertex ret(
		       (this->x - v[0]), 
		       (this->y - v[1]), 
		       (this->z - v[2])
               );

	return (float)sqrt(
					   (ret.x * ret.x) +
					   (ret.y * ret.y) +
				       (ret.z * ret.z)
					  );
}

Vertex::~Vertex()
{

}


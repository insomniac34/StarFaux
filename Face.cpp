//written by Tyler Raborn

#include <cstdio>
#include <vector>
#include <cmath>
#include "Face.h"
#include "Vertex.h"
#include "matrix.h"

//CLASS FACE DEF

Face::Face()
{

}

Face::Face(Vertex p0, Vertex p1, Vertex p2)
{
	facepoint0 = &p0;
	facepoint1 = &p1;
	facepoint2 = &p2;

	size = 3;
}

//this constructor takes in pointers to the 4 points that make up a given rectangular face
Face::Face(Vertex *p0, Vertex *p1, Vertex *p2, Vertex *p3)
{
	
	facepoint0 = p0 + 0;
	facepoint1 = p1 + 0;
	facepoint2 = p2 + 0;
	facepoint3 = p3 + 0;
}

void Face::print() //for testing
{

}

void Face::draw(int renderMode)
{
	glBegin(renderMode);
	
	glVertex4f(facepoint0->x, facepoint0->y, facepoint0->z, facepoint0->w);
	glVertex4f(facepoint1->x, facepoint1->y, facepoint1->z, facepoint1->w);
	glVertex4f(facepoint2->x, facepoint2->y, facepoint2->z, facepoint2->w);
	glVertex4f(facepoint3->x, facepoint3->y, facepoint3->z, facepoint3->w);

	glEnd();
}

Vertex Face::getFP0()
{
	return Vertex(facepoint0->x, facepoint0->y, facepoint0->z);
}

Vertex Face::getFP1()
{
	return Vertex(facepoint1->x, facepoint1->y, facepoint1->z);
}

Vertex Face::getFP2()
{
	return Vertex(facepoint2->x, facepoint2->y, facepoint2->z);
}

Vertex Face::getFP3()
{
	return Vertex(facepoint3->x, facepoint3->y, facepoint3->z);
}

//destructor
Face::~Face()
{

}
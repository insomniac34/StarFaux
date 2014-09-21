/* @Programmer: Tyler Raborn
 * @Course: CS 1566 
 * @Assignment: Assignment 5 - Raytracer
 * @Description: Header file for my Shape class for holding & maintaining data on 3D shapes.
 * @Dev Environment: Ubuntu Linux 13.10 (amd64), g++ 4.7
 * @GPU: nVidia GeForce GTX 670 2GB
 */

#define SHAPE_CUBE 0
#define SHAPE_CYLINDER 1
#define SHAPE_SPHERE 2
#define SHAPE_TORUS 3
#define SHAPE_CONE 4

#define LINK_MASTER 0
#define LINK_SLAVE 1

#include <cmath>
#include <cstdio>

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
#include "matrix.h"
#include <deque>
#include <vector>
#include "Vertex.h"
#include "Face.h"
#include "Camera.h"
#include "vec3.h"

#ifndef _SHAPE_H
#define _SHAPE_H

class Shape
{

public:	
	Shape(); 
	explicit Shape(int, int, int, GLdouble, const GLfloat*); //This constructor initializes all necessary fields AND performs calculations on params to generate & store vertices
	explicit Shape(Shape*); //copy constructor
	~Shape();

	//accessors/mutators
	const int getType();
	float **getCTM();
	void draw(int);
	//Vertex *getPosition();
	GLfloat *getPosition();

	//methods for vertex manipulation:
	void tessellate(); 
	void translate(GLfloat, GLfloat, GLfloat);
	void translateShapeLocalized(GLfloat, int, int, int);
	void scale(GLfloat, GLfloat, GLfloat);
	void rotate(GLfloat, GLfloat, GLfloat, GLfloat);
	void revolve(GLfloat, GLfloat, GLfloat, GLfloat);
	void arbitraryRotate(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
	void deleteMatrix(GLdouble**);
	void setVS(int);
	void setColor(GLfloat*);

	//methods for Camera/Object inter-class operations:
	friend void Camera::synchronizedRotate(const GLdouble, Shape*);
	friend void Camera::synchronizedRotate(const GLdouble, std::deque<Shape*>);
	friend void Camera::synchronizedTranslate(const GLdouble, const GLdouble, const GLdouble, Shape*);
	friend void Camera::synchronizedTranslate(const GLdouble, const GLdouble, const GLdouble, std::deque<Shape*>);

	//ray tracing methods for  project 5
	void paintXAxis(); //draw a ray originating at this->position and extending to edge of world
	GLdouble* isHit(Vertex, Vertex); 
	bool* fireRay(std::deque<Shape>*, std::deque<Shape>*);

	//methods for linking objects
	void link(Shape*);
	void link(std::deque<Shape*>);

	GLdouble *getAxes();
	GLfloat *getYAxis();
	GLfloat rgb[3]; //array containing 3 rgb color values {red, green, blue}

	//"extra features":
	void jump();
	
private:

	int rs, vs;
	int type; //0 = cube, 1 = cylinder, 2 = sphere, 3 = torus
	GLdouble CTM[4][4]; //Cumulative Transformation Matrix
	GLdouble CTMinverse[4][4];
	GLdouble CTMtrans[4][4];
	GLdouble CTMdir_inverse[4][4]; //for direction

	//fields & methods for object linking...
	int LINK_STATE;
	void SET_MASTER();
	std::deque<Shape*> linkArray;

	//axes
	Vertex position; //current center point
	Vertex curXAxis; //current X axis
	Vertex curAxis;  //current Y axis
	Vertex curZAxis; /// current Z axis

	//core data structures:
	std::vector<std::vector<Vertex> > vertexArray;
	std::vector<Vertex> normArray;
	std::vector<std::vector<Face> > quadArray;

	GLdouble dy;
	bool rest;
	GLdouble shapeHeight;

	//private methods:
	void drawCube(int, bool); //contains necessary calculations to draw a tessellated 3d cube
	void drawCylinder(int, bool); //contains necessary calculations to draw a tessellated 3d cylinder
	void drawSphere(int, bool); //contains necessary calculations to draw a tessellated 3d sphere
	void drawTorus(int, bool); //contains necessary calculations to draw a tessellated 3d torus
	void drawCone(int, bool); //contains necessary calculations to draw a tessellated 3d cone
};

#endif
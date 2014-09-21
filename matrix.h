/*@Programmer: Tyler Raborn
 *@Descrip: Header file for my collection of matrix algebra functions.
*/

//MUST make sure that the Apple Magical Fairy Princess is sufficiently pleased -tyler
#include <cmath>
#include <cstdio>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Vertex.h"

#ifndef _MATRIX_H
#define _MATRIX_H

GLdouble* translateVertexY(GLdouble, Vertex); //generates the 4x4 transformation matrix utilizing homogenous coordinates {x, y, z, w} on a 3D plane.

GLdouble* rotateVertexY(GLdouble, Vertex);

GLdouble* scalarVectormult4d(GLdouble, GLdouble[4]); //accepts a scalar and a 1x4 column vector as parameters; returns the 1x4 column vector that results from scalar multiplication on the two parameters.

GLdouble** matrixMult4d(GLdouble[4][4], GLdouble[4][4], Vertex); //Accepts a pair of two-dimensional double arrays A & B representing 4x4 matrices; returns the 4x4 matrix resulting from A*B.



GLdouble** matrixMult(GLdouble[][4], GLdouble[][4]);

//GLdouble** matrixMult(GLdouble**, GLdouble**);

GLdouble** matrixMult(GLdouble[][4], GLdouble**);

GLdouble** matrixMult(GLdouble**, GLdouble[4][4]);



GLdouble** matrixAdd4d(GLdouble[4][4], GLdouble[4][4]); //Accepts a pair of two-dimensional double arrays A & B representing 4x4 matrices; returns the 4x4 matrix resulting from A+B.

GLdouble* triMatrixVectorMult4d(double[4][4], double[4][4], double[4][4], double V[4]);

GLdouble dotProduct4d(GLdouble[4], GLdouble[4]);

Vertex* crossProduct4d(GLfloat[4], GLfloat[4]);

GLdouble* crossProduct(GLdouble a[3], GLdouble b[3]);

GLdouble* crossProduct3V(Vertex v1, Vertex v2, Vertex V3);

GLdouble* normalize(Vertex vector);

GLdouble* translateVertex(Vertex, GLdouble, GLdouble, GLdouble); //multiplies vertices by translation matrix specified by the x y and z coordinate params

GLdouble* vectorMatrixMult(Vertex, GLdouble[4][4]);

GLdouble* rotateVertex(Vertex v, GLdouble deg, GLdouble x, GLdouble y, GLdouble z);

GLdouble* rotateVertexLocalized(Vertex, GLdouble, GLdouble, GLdouble, GLdouble);

GLdouble* scaleVertex(Vertex, GLdouble, GLdouble, GLdouble);

void printMatrix(GLdouble**);

void printMatrix(GLdouble[4][4]);

GLdouble* getZeroes(GLdouble, GLdouble, GLdouble);

#endif

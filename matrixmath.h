/****************************************************
 FILE:		matrixmath.h
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		October 26, 2013
 DESC:		Header file for matrixmath.cpp
 ****************************************************/

/*
 All of the functions (with the exception of the normalize function)
 take two inputs and record the result in a third parameter.
 The third parameter may also be one of the inputs.
 */

#ifndef _MATRIXMATH_H_
#define _MATRIXMATH_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;

// initialize a vector (all zeroes)
void init_vector(GLfloat v[4]);

// initialize a matrix (identity matrix)
void load_identity_matrix(GLfloat m[16]);

// vector addition
void vv_add(GLfloat v1[4], GLfloat v2[4], GLfloat res[4]);

// vector subtraction
void vv_sub(GLfloat v1[4], GLfloat v2[4], GLfloat res[4]);

// vector/matrix dot product
void vm_dot(GLfloat v[4], GLfloat m[16], GLfloat res[4]);

// vector/vector dot product
float vv_dot(GLfloat v1[4], GLfloat v2[4]);

// vector magnitude
float v_len(GLfloat v[4]);

// matrix/matrix dot product
void mm_dot(GLfloat m1[16], GLfloat m2[16], GLfloat res[16]);

// vector/vector cross product
void vv_cross(GLfloat v1[4], GLfloat v2[4], GLfloat res[4]);

// vector normalization
void normalize(GLfloat v1[4]);

// copies results of an operation to the result variable
void copy(GLfloat* r1, GLfloat* r2, int size);

#endif
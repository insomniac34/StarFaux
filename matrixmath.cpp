/****************************************************
 FILE:		matrixmath.cpp
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		October 26, 2013
 DESC:		Module for vector and matrix operations
 ****************************************************/

#include <cmath>
#include "matrixmath.h"

// initialize a vector (all zeroes)
void init_vector(GLfloat v[4])
{
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
	v[3] = 1.0;
}

// initialize a matrix (identity matrix)
void load_identity_matrix(GLfloat m[16])
{
	m[0] = 1.0;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	
	m[4] = 0.0;
	m[5] = 1.0;
	m[6] = 0.0;
	m[7] = 0.0;
	
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 1.0;
	m[11] = 0.0;
	
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}

// vector addition
void vv_add(GLfloat v1[4], GLfloat v2[4], GLfloat res[4])
{
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
	res[3] = 1.0;
}

// vector subtraction
void vv_sub(GLfloat v1[4], GLfloat v2[4], GLfloat res[4])
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
	res[3] = 1.0;
}

// vector/matrix dot product
void vm_dot(GLfloat v[4], GLfloat m[16], GLfloat res2[4])
{
	GLfloat res[4];
	int row = 0;
	for( int i = 0; i < 4; i++ )
	{
		row = i * 4;
		res[i] = m[row+0]*v[0] + m[row+1]*v[1] + m[row+2]*v[2] + m[row+3]*v[3];
	}
	copy(res, res2, 4);
}

// vector/vector dot product
float vv_dot(GLfloat v1[4], GLfloat v2[4])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

// vector magnitude
float v_len(GLfloat v[4])
{
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

// matrix/matrix dot product
void mm_dot(GLfloat m1[16], GLfloat m2[16], GLfloat res2[16])
{
	GLfloat res[16];
	int row;
	for( int i = 0; i < 4; i++ )
	{
		row = i * 4;
		for( int j = 0; j < 4; j++ )
		{
			res[row+j] = m1[row+0] * m2[0+j] + m1[row+1] * m2[4+j] + m1[row+2] * m2[8+j] + m1[row+3] * m2[12+j];
		}
	}
	
	copy(res, res2, 16);
}

// vector/vector cross product
void vv_cross(GLfloat v1[4], GLfloat v2[4], GLfloat res2[4])
{
	GLfloat res[4];
	
	res[0] = v1[1] * v2[2] - v1[2] * v2[1];
	res[1] = v1[2] * v2[0] - v1[0] * v2[2];
	res[2] = v1[0] * v2[1] - v1[1] * v2[0];
	res[3] = 1.0;
	
	copy(res, res2, 4);
}

// vector normalization
void normalize(GLfloat v1[4])
{
	GLfloat v_len = sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2));
	v1[0] = v1[0]/v_len;
	v1[1] = v1[1]/v_len;
	v1[2] = v1[2]/v_len;
	v1[3] = 1.0;
}

void copy(GLfloat* r1, GLfloat* r2, int size)
{
	for( int i = 0; i < size; i++)
	{
		r2[i] = r1[i];
	}
}
/********************************************************************************
* @Developer: Tyler Raborn
* @Course: CS1566 Introduction to Graphics
* @Description: A set of functions designed to provide basic linear algebra capabilities for OpenGL calculations.
* @Dev Environment: Microsoft Visual Studio 2012, Windows 8 Pro 64-bit
* @GPU: nVidia GeForce GTX 670 2GB
*********************************************************************************/


#define dw 1.0
#define _USE_MATH_DEFINES

#include "matrix.h"

#define RADIANS M_PI / 180.0

//GLOBALS
static GLdouble identity4d[4][4] = {
									{1.0, 0.0, 0.0, 0.0},
									{0.0, 1.0, 0.0, 0.0},
									{0.0, 0.0, 1.0, 0.0},
									{0.0, 0.0, 0.0, 1.0},
								   };

static GLdouble zeroMatrix[4][4] = {
							     	{0.0, 0.0, 0.0, 0.0},
								    {0.0, 0.0, 0.0, 0.0},
								    {0.0, 0.0, 0.0, 0.0},
								    {0.0, 0.0, 0.0, 0.0},
								   };

void printMatrix(GLdouble** M)
{
	printf("\n{\n");
	for (int i = 0; i < 4; i++)
	{
		printf(" {");
		for (int j = 0; j < 4; j++)
		{
			printf("%g, ", M[i][j]);
		}
		printf("},\n");
	}
	printf("};\n");
}

void printMatrix(GLdouble M[4][4])
{
	printf("{\n");
	for (int i = 0; i < 4; i++)
	{
		printf(" {");
		for (int j = 0; j < 4; j++)
		{
			printf("%g, ", M[i][j]);
		}
		printf("},\n");
	}
	printf("};\n");
}

GLdouble* translateVertexY(GLdouble d, Vertex v)
{
	GLdouble T[4][4] = {
						{1.0, 0.0, 0.0, 0.0},
						{0.0, 1.0, 0.0,   d}, 
						{0.0, 0.0, 1.0, 0.0},
						{0.0, 0.0, 0.0,  dw}								
					   };

	GLdouble* ret = new GLdouble[4]; //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
	GLdouble** temp = new GLdouble*[4];
	temp[0] = new GLdouble[4];
	temp[1] = new GLdouble[4];
	temp[2] = new GLdouble[4];
	temp[3] = new GLdouble[4];

	int i;
	for (i = 0; i < 4; i++)
	{		
		temp[i][0] = (T[i][0]*(v.x));
		temp[i][1] = (T[i][1]*(v.y)); 
		temp[i][2] = (T[i][2]*(v.z)); 
		temp[i][3] = (T[i][3]*(v.w)); 

		ret[i] = (temp[i][0] + temp[i][1] + temp[i][2] + temp[i][3]);
	}

	//take out the trash:
	delete[] temp[0];
	delete[] temp[1];
	delete[] temp[2];
	delete[] temp[3];
	delete[] temp; 

	return ret;
}

GLdouble* rotateVertexY(GLdouble THETA, Vertex v) //accepts a number THETA and a 1x4 column vector as parameters; returns the column vector resulting from using the 3D 4x4 rotation matrix to rotate the initial vector THETA degrees about the Y-axis. 
{
	THETA*=RADIANS;
	GLdouble rotator[4][4] = {
						   	  {cos(THETA),      0.0,      sin(THETA),      0.0}, 
							  {0.0,             1.0,      0.0,             0.0}, 
							  {(-1*sin(THETA)), 0.0,      cos(THETA),      0.0}, 
							  {0.0,             0.0,      0.0,             1.0}
	                         };

	GLdouble* ret = new GLdouble[4]; //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
	GLdouble** temp = new GLdouble*[4];
	temp[0] = new GLdouble[4];
	temp[1] = new GLdouble[4];
	temp[2] = new GLdouble[4];
	temp[3] = new GLdouble[4];

	int i;
	for (i = 0; i < 4; i++) //for each ROW of the rotation matrix
	{
		temp[i][0] = (rotator[i][0] * v.x);
		temp[i][1] = (rotator[i][1] * v.y);
		temp[i][2] = (rotator[i][2] * v.z);
		temp[i][3] = (rotator[i][3] * v.w);

		ret[i] = (temp[i][0] + temp[i][1] + temp[i][2] + temp[i][3]);
	}
	
	//take out the trash:
	delete[] temp[0];
	delete[] temp[1];
	delete[] temp[2];
	delete[] temp[3];
	delete[] temp; 

	return ret;
}

//accepts a scalar and a 1x4 column vector as parameters; returns the 1x4 column vector that results from scalar multiplication on the two parameters.
GLdouble* scalarVectormult4d(GLdouble scalar, GLdouble vector[4]) 
{
	GLdouble* ret = new GLdouble[4];

	int i;
	for (i = 0; i < 4; i++)
	{
		ret[i] = (scalar * vector[i]); 
	}

	return ret; //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
}

//accepts a trio if two-dimensional arrays representing 4x4 matrices, as well as a 4x1 column vector V. Returns the result of v*C*B*A
GLdouble* triMatrixVectorMult4d(GLdouble A[4][4], GLdouble B[4][4], GLdouble C[4][4], GLdouble V[4])
{
	GLdouble *ret, *temp0, *temp1, *ret0, *ret1;
	ret = new GLdouble[4];
	temp0 = new GLdouble[4];
	temp1 = new GLdouble[4];
	ret0 = new GLdouble[4];
	ret1 = new GLdouble[4];

	for(int q = 0; q < 4; q++) //memset everything to 0's!
	{
		ret1[q] = 0.0;
		ret0[q] = 0.0;
		ret[q] = 0.0;
		temp0[q] = 0.0;
		temp1[q] = 0.0;
	}

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
			temp1[i] += (C[j][i] * V[i]);
		for (int k = 0; k < 4; k++)
			ret[j] += temp1[k];

		//reset values to 0
		for(int q = 0; q < 4; q++)
			temp1[q] = 0.0;
		for(int q = 0; q < 4; q++)
			temp0[q] = 0.0;
	}

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
			temp1[i] += (B[j][i] * ret[i]);
		for (int k = 0; k < 4; k++)
			ret0[j] += temp1[k];

		//reset values to 0
		for(int q = 0; q < 4; q++)
			temp1[q] = 0.0;
		for(int q = 0; q < 4; q++)
			temp0[q] = 0.0;
	}

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
			temp1[i] += (A[j][i] * ret0[i]);
		for (int k = 0; k < 4; k++)
			ret1[j] += temp1[k];

		//reset values to 0
		for(int q = 0; q < 4; q++)
			temp1[q] = 0.0;
		for(int q = 0; q < 4; q++)
			temp0[q] = 0.0;
	}

	//take out the trash
	delete[] temp0;
	delete[] temp1;
	delete[] ret; 
	delete[] ret0;

	return ret1;	
}

//Accepts a pair of two-dimensional double arrays A & B representing 4x4 matrices, as well as a Vector V; returns the 1x4 matrix resulting from (A*B)*V.
GLdouble** matrixMult4d(GLdouble A[4][4], GLdouble B[4][4])
{
	GLdouble* temp = new GLdouble[4];
	GLdouble** ret = new GLdouble*[4];
	for (int i = 0; i < 4; i++)
		ret[i] = new GLdouble[4];

	for (int i = 0; i < 4; i++)//for i = each row in A
	{
		for (int j = 0; j < 4; j++) //for j = each column in B
			temp[i] += (A[i][j] * B[j][i]);

		for (int k = 0; k < 4; k++) //for k = each value in temp
			ret[i][k] = temp[k];
	}

	delete[] temp;

	return ret;
}

GLdouble** matrixMult(GLdouble A[][4], GLdouble B[][4]) //version for usage w/ 2D matrix
{
	//printf("[][4], [][4]\n");
    GLdouble** ret = new GLdouble*[4];
    for (int i = 0; i < 4; i++)
    {
        ret[i] = new GLdouble[4];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            GLdouble temp = 0;
            for (int k = 0; k < 4; k++)
            {
                temp+=(A[i][k]*B[k][j]);
            }
            ret[i][j] = temp;
        }
    }
    return ret;
}

GLdouble** matrixMult(GLdouble A[][4], GLdouble **B) //version for usage with double pointer
{
	//printf("[][4], **\n");
    GLdouble** ret = new GLdouble*[4];
    for (int i = 0; i < 4; i++)
    {
        ret[i] = new GLdouble[4];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            GLdouble temp = 0;
            for (int k = 0; k < 4; k++)
            {
                temp+=(A[i][k]*B[k][j]);
            }
            ret[i][j] = temp;
        }
    }
    return ret;
}

GLdouble** matrixMult(GLdouble **A, GLdouble B[4][4]) //version for usage with double pointer
{
    GLdouble** ret = new GLdouble*[4];
    for (int i = 0; i < 4; i++)
    {
        ret[i] = new GLdouble[4];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            GLdouble temp = 0;
            for (int k = 0; k < 4; k++)
            {
                temp+=(A[i][k]*B[k][j]);
            }
            ret[i][j] = temp;
        }
    }
    return ret;
}

GLdouble* vectorMatrixMult(Vertex V, GLdouble A[4][4])
{
	GLdouble* ret = new GLdouble[4];
	for (int i = 0; i < 4; i++)
	{
		ret[i] = ((A[i][0]*V.x) + (A[i][1]*V.y) + (A[i][2]*V.z) + (A[i][3]*V.w));
	}
	return ret;
}

//Accepts a pair of two-dimensional GLdouble arrays A & B representing 4x4 matrices; returns the 4x4 matrix resulting from A+B.
GLdouble** matrixAdd4d(GLdouble **A, GLdouble **B) //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
{
	GLdouble** ret = new GLdouble*[4];
	ret[0] = new GLdouble[4];
	ret[1] = new GLdouble[4];
	ret[2] = new GLdouble[4]; 
	ret[3] = new GLdouble[4];

	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			ret[i][j] = (A[i][j] + B[i][j]);
		}
	}

	return ret;
}

//accepts two 4x1 column vectors as parameters; returns their dot product
GLdouble dotProduct4d(GLdouble a[4], GLdouble b[4])
{
	GLdouble ret = 0;
	
	for (int i = 0; i < 4; i++)
	{
		ret+=(a[i]*b[i]);
	}	
	
	return ret;
}

//accepts a pair of 1x4 column vectors as parameters; returns the 4x1 column vector that lies orthogonal to a and b as a vertex object
Vertex* crossProduct4d(Vertex a, Vertex b)
{
	GLdouble* temp = new GLdouble[3];
	
	temp[0] = ((a.y * b.z) - (a.z * b.y)); //aybz - azby
	temp[1] = ((a.z * b.x) - (a.x * b.z)); //azbx - axbz
	temp[2] = ((a.x * b.y) - (a.y * b.x)); //axby - aybx

	Vertex* ret = new Vertex(temp);

	delete[] temp;

	return ret; //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
}

GLdouble* crossProduct(GLdouble a[3], GLdouble b[3])
{
	GLdouble* ret = new GLdouble[3];

	ret[0] = ((a[1]*b[2]) - (a[2]*b[1]));
	ret[1] = ((a[2]*b[0]) - (a[0]*b[2]));
	ret[2] = ((a[0]*b[1]) - (a[1]*b[0]));

	return ret;
}

//returns the cross product result of [v1-v2]*[v2-v3]
GLdouble* crossProduct3V(Vertex v1, Vertex v2, Vertex v3)
{
	Vertex a, b;
	a = Vertex((v1.x-v2.x), (v1.y-v2.y), (v1.z-v2.z));
	b = Vertex((v2.x-v3.x), (v2.y-v3.y), (v2.z-v3.z));

	GLdouble* temp = new GLdouble[3];
	
	temp[0] = ((a.y * b.z) - (a.z * b.y)); //aybz - azby
	temp[1] = ((a.z * b.x) - (a.x * b.z)); //azbx - axbz
	temp[2] = ((a.x * b.y) - (a.y * b.x)); //axby - aybx

	return temp; //WARNING: RetVal is allocated on the HEAP!!! Callee variable deallocation required!
}

GLdouble* normalize(Vertex vec)
{
	GLdouble* ret = new GLdouble[3];
	GLdouble length = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));

	ret[0] = vec.x/length;
	ret[1] = vec.y/length;
	ret[2] = vec.z/length;

	return ret;
}

//translates some vertex V by amount provided by x/y/z 
GLdouble* translateVertex(Vertex v, GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble translate[4][4] = {
								{1.0, 0.0, 0.0,  x},
								{0.0, 1.0, 0.0,  y},
								{0.0, 0.0, 1.0,  z},
								{0.0, 0.0, 0.0, dw},
							   };

	GLdouble* ret = vectorMatrixMult(v, translate);

	return ret; 
}

//Accepts a vertex V, a number THETA, and a flag determining which axis to rotate around. Returns the coordinates of v rotated THETA degrees about the N-axis, where N=whichever of x/y/z is 1
GLdouble* rotateVertex(Vertex v, GLdouble THETA, GLdouble x, GLdouble y, GLdouble z)
{
	THETA*=RADIANS;
	//printf("ROTATING: %g, %g, %g\n", x, y, z);
	GLdouble* ret = {0};
	if (x==1 || x==-1) //rotate THETA degrees via X rotation matrix
	{
		GLdouble rotateX[4][4] = {
								  {1.0, 0.0,		  0.0,	             0.0}, 
								  {0.0, cos(x*THETA), (-1*sin(x*THETA)), 0.0}, 
								  {0.0, sin(x*THETA), cos(x*THETA),      0.0}, 
								  {0.0, 0.0,	      0.0,	             1.0}, 
								 };

		ret = vectorMatrixMult(v, rotateX);
	}
	else if (y==1 || y==-1) //rotate THETA degrees via Y rotation matrix
	{
		GLdouble rotateY[4][4] = {
								  {cos(y*THETA),       0.0, sin(y*THETA), 0.0}, 
							      {0.0,                1.0, 0.0,          0.0}, 
							      {(-1*sin(y*THETA)),  0.0, cos(y*THETA), 0.0}, 
							      {0.0,                0.0, 0.0,          1.0}, 
						         };

		ret = vectorMatrixMult(v, rotateY);
	}
	else if (z==1 || z==-1) //rotate THETA degrees via Z rotation matrix
	{
		GLdouble rotateZ[4][4] = {
								  {cos(z*THETA), (-1*sin(z*THETA)), 0.0, 0.0}, 
							      {sin(z*THETA), cos(z*THETA),      0.0, 0.0}, 
							      {0.0,			 0.0,               1.0, 0.0}, 
							      {0.0,			 0.0,               0.0, 1.0}, 
						         };

		ret = vectorMatrixMult(v, rotateZ);
	}
	else
	{
		ret = vectorMatrixMult(v, identity4d);
	}

	return ret;
}

//Rotates the given vertex V about the specified local axis...
GLdouble* rotateVertexLocalized(Vertex v, GLdouble THETA, GLdouble x, GLdouble y, GLdouble z)
{
	THETA*=RADIANS;
	GLdouble* ret;
	
	GLdouble M[4][4] =	{ 
						 {(pow(x, 2)+(cos(THETA)*(1-pow(x, 2)))),	 (x*(y*(1-cos(THETA)))-(z*sin(THETA))),	   (z*(x*(1-cos(THETA)))+(y*sin(THETA))),   0.0},
						 {(x*(y*(1-cos(THETA)))+(z*sin(THETA))),     (pow(y, 2)+(cos(THETA)*(1-pow(y, 2)))),   (y*(z*(1-cos(THETA)))-(x*sin(THETA))),   0.0},
						 {(z*(x*(1-cos(THETA)))-(y * sin(THETA))),	 (y*(z*(1-cos(THETA)))+(x*sin(THETA))),	   (pow(z, 2)+(cos(THETA)*(1-pow(z, 2)))),  0.0}, 
						 {0.0,									     0.0,									   0.0,                                     1.0}, 
						};

	ret = vectorMatrixMult(v, M);
    return ret;
}

//scales the passed in vertex V by Sx/Sy/Sz
GLdouble* scaleVertex(Vertex v, GLdouble Sx, GLdouble Sy, GLdouble Sz)
{
	GLdouble* ret;
	GLdouble S[4][4] = {
						{Sx, 0.0, 0.0,  0.0}, 
						{0.0, Sy, 0.0,  0.0}, 
						{0.0, 0.0, Sz,  0.0}, 
						{0.0, 0.0, 0.0, 1.0},
	                   };
	                   
	ret = vectorMatrixMult(v, S);
	return ret;
}

//evaluates the Quadratic Equation on the passed-in a, b, c values, returns an array containing the two zeroes and a flag
GLdouble *getZeroes(GLdouble a, GLdouble b, GLdouble c)
{
    GLdouble *ret = new GLdouble[3];
    GLdouble test_pos, test_neg;

    if ((2*a)!=0)
    {
        if (pow(b, 2) - (4*a*c) >= 0)
        {
            test_pos = ((-1 * b) + sqrt(pow(b, 2) - (4*a*c)));
            test_neg = ((-1 * b) - sqrt(pow(b, 2) - (4*a*c)));
            
            if (test_pos==test_neg)
            {
                ret[0] = test_pos / (2*a);
                ret[1] = 0.0;
                ret[2] = 1; //flag = valid; one zero 
            }
            else
            {
                ret[0] = test_pos / (2*a);
                ret[1] = test_neg / (2*a);
                ret[2] = 2; //flag = valid; two zeroes
            }
        }
        else
        {
            ret[0] = 0.0;
            ret[1] = 0.0;
            ret[2] = 3; //flag = undefined 
        }
    }
    else
    {
        ret[0] = 0.0;
        ret[1] = 0.0;
        ret[2] = 0; //flag = dne
    }

    return ret;
}
/********************************************************************************
* @Developer: Tyler Raborn
* @Course: CS1566 
* @Description: Methods & Constructors for my Shape class
* @Dev Environment: Microsoft Visual Studio 2012 + Windows 8 Pro 64-bit, g++ 4.8 + Ubuntu Linux 13.10
* @GPU: nVidia GeForce GTX 670 2GB
*********************************************************************************/

#define dw 1.0
#define RADIANS M_PI / 180.0
#define _USE_MATH_DEFINES

#include "Shape.h"
//#include "Camera.h"
//#include "mymath.h"

static const float r1 = 1.2;
static const float r2 = 0.2;
static const float r = 1.0;
static const float Rcyl = 1;
static const int MAX_RS = 50;
static const int MAX_VS = 50;
GLfloat targetColor[3] = {1.0, 0.0, 0.0};

static const GLdouble identity4d[4][4] = {
									      {1.0, 0.0, 0.0, 0.0},
									      {0.0, 1.0, 0.0, 0.0},
									      {0.0, 0.0, 1.0, 0.0},
									      {0.0, 0.0, 0.0, 1.0},
								         };

static const GLdouble zeroMatrix[4][4] = {
							     	      {0.0, 0.0, 0.0, 0.0},
								          {0.0, 0.0, 0.0, 0.0},
								          {0.0, 0.0, 0.0, 0.0},
								          {0.0, 0.0, 0.0, 0.0},
								         };

Shape::Shape()
{
	//it is Sacrilage to defile thy Default Constructor
}

Shape::Shape(int shapeType, int rs, int vs, GLdouble maxHeight, const GLfloat* color) //vs = vertical stacks, rs = radial stacks
{
	this->shapeHeight = maxHeight;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i==j) 
			{
				CTM[i][j] = 1.0;
				CTMinverse[i][j] = 1.0;
				CTMdir_inverse[i][j] = 1.0;
			}
			else
			{
				CTM[i][j] = 0.0;
				CTMinverse[i][j] = 0.0;
				CTMdir_inverse[i][j] = 0.0;
			}
		}
	}

	this->rgb[0] = color[0];
	this->rgb[1] = color[1];
	this->rgb[2] = color[2];

	this->rs = rs;
	this->vs = vs;
	type = shapeType;
	rest=false;

	position = Vertex(0.0, 0.0, 0.0);
	curXAxis = Vertex(1.0, 0.0, 0.0);
	curAxis  = Vertex(0.0, 1.0, 0.0); //y
	curZAxis = Vertex(0.0, 0.0, 1.0);

	for (int i = 0; i < MAX_VS; i++) //initialize data structures
	{
		vertexArray.push_back(std::vector<Vertex>());
		quadArray.push_back(std::vector<Face>());
	}

	if (type==0) //cube
	{
		maxHeight/=1.40;
		int i;  
		for(i = 0; i < 4; i++)
		{
			Vertex v0 = Vertex(
				               maxHeight*cos((M_PI/4)), 
				               -maxHeight*sin(i*(M_PI/2)+(M_PI/4)), 
				               maxHeight*cos(i*(M_PI/2)+(M_PI/4))
				              );

			Vertex v1 = Vertex(
				               -maxHeight*cos((M_PI/4)),
			                   -maxHeight*sin(i*(M_PI/2)+(M_PI/4)), 
			                   maxHeight*cos(i*(M_PI/2)+(M_PI/4))
			                  );

			vertexArray[i].push_back(v0);
			vertexArray[i].push_back(v1);
		}
		this->vs = 4;
		this->rs = 2;
	}
	else if (type == 1) //cylinder
	{
		//printf("entering makeCylinder...\n");
		
		maxHeight=.5;
		
		int i = 0;
		int finalh;
		GLdouble h;
		GLdouble THETA;
		for (h = 0; h < maxHeight; h += (maxHeight / vs)) //outer loop handles height change
		{
			for (THETA = 0.0; THETA < (2*M_PI); THETA += ((2*M_PI) / rs)) //inner loop handles circles
			{		
				Vertex curPos = Vertex(Rcyl*cos(THETA), h, (-Rcyl * sin(THETA))); //current position on unit circle

				GLdouble* rotResult = rotateVertexY(2*M_PI, curPos);
				Vertex rotate = Vertex(rotResult);
				GLdouble* transResult = translateVertexY(h, rotate);
				Vertex transform = Vertex(transResult); //translate up w/ respect to the Y axis via translation matrix

				vertexArray[i].push_back(transform);

				delete[] rotResult;
				delete[] transResult;
			}
			i++;
			finalh = h;
		}
		if (i != MAX_VS)
		{
			while (vs >= i) //correction loop
			{
				finalh += (maxHeight / vs);
				Vertex curPos = Vertex(Rcyl*cos(THETA), finalh, (-Rcyl * sin(THETA))); //current position on unit circle

				GLdouble* rotateResult = rotateVertexY(2*M_PI, curPos);
				Vertex rotate = Vertex(rotateResult); //apply rotation matrix
				GLdouble* transformResult = translateVertexY(finalh, rotate); 
				Vertex transform = Vertex(transformResult); //translate up w/ respect to the Y axis via translation matrix

				vertexArray[i].push_back(transform);

				delete[] rotateResult;
				delete[] transformResult;

				i++;
			}
		}
		//printf("vertexArray calculations for CYLINDER complete: size of vertexArray: %d, memory address of FRONT object in vertexArray: %g\n", vertexArray.size()-1, &vertexArray.front());
		//this->translate(0, -.5, 0);
	}

	else if (type == 2) //sphere, centered at the origin...sph_ray = 3 vs = 10, rs = 20
	{
		GLdouble PHI, THETA;
		THETA = 0.0;
		PHI = 0.0;

		for (int i = 0; i < vs; i++) //Vertical Stacks
		{
			for (int j = 0; j < rs; j++) //Radial Slices
			{
				GLdouble V[4] = {r, 0.0, 0.0, 1.0};

				GLdouble Rz[4][4] = {
									 {cos(THETA),    (-1 * sin(THETA)),   0.0, 0.0},
						             {sin(THETA),    cos(THETA),          0.0, 0.0},
						             {0.0,           0.0,                 1.0, 0.0},
						             {0.0,           0.0,                 0.0, 1.0},
					                };

				GLdouble Ry[4][4] = {
									 {cos(PHI),            0.0,     sin(PHI),  0.0}, 
									 {0.0,                 1.0,     0.0,       0.0},
									 {(-1 * sin(PHI)),     0.0,     cos(PHI),  0.0},
									 {0.0,                 0.0,     0.0,       1.0},
									};

				GLdouble* result0 = vectorMatrixMult(V, Ry);
				GLdouble* result1 = vectorMatrixMult(Vertex(result0), Rz);
				
				vertexArray[i].push_back(Vertex(result1));
				
				delete[] result0; 
				delete[] result1;

				PHI+=((2*M_PI) / (rs)); //2pi / rs for default shitty sphere
			}
			THETA+=((M_PI) / (vs-1)); //M_PI / vs for default shitty sphere
		}
	}

	else if (type == 3) //torus
	{
		GLdouble PHI, THETA;
		THETA = 0.0;//((2*M_PI) / vs);
		PHI = 0.0;//((2*M_PI) / rs);
		for (int i = 0; i < vs; i++)
		{
			for (int j = 0; j < rs; j++)
			{
				GLdouble V[4] = {r2, 0.0, 0.0, 1.0};

				GLdouble Rz[4][4] = {
									 {cos(THETA),    (-1 * sin(THETA)),   0.0, 0.0},
						             {sin(THETA),    cos(THETA),          0.0, 0.0},
						             {0.0,           0.0,                 1.0, 0.0},
						             {0.0,           0.0,                 0.0, 1.0},
					                };

				GLdouble Ry[4][4] = {
									 {cos(PHI),            0.0,     sin(PHI),  0.0}, 
									 {0.0,                 1.0,     0.0,       0.0},
									 {(-1 * sin(PHI)),     0.0,     cos(PHI),  0.0},
									 {0.0,                 0.0,     0.0,       1.0},
									};

				 GLdouble T[4][4] = {
									 {1.0, 0.0, 0.0,  r1},
									 {0.0, 1.0, 0.0, 0.0},
									 {0.0, 0.0, 1.0, 0.0},
									 {0.0, 0.0, 0.0, 1.0},
								    };

				 GLdouble* result0 = vectorMatrixMult(V, Rz);
				 GLdouble* result1 = vectorMatrixMult(Vertex(result0), T);
				 GLdouble* result2 = vectorMatrixMult(Vertex(result1), Ry);

				 vertexArray[i].push_back(Vertex(result2));

				 delete[] result0; 
				 delete[] result1;
				 delete[] result2;

				PHI+=((2*M_PI) / rs);
			}
			THETA+=((2*M_PI) / vs);
		}
	} 

	else if (type==4) //cone
	{
		int finalh; //holds the value of h after loop completion for use in correction; converts to int to ensure consistent intervals in correction loop.
		int i = 0;
		GLdouble h;
		GLdouble THETA;
		for (h = 0; h < maxHeight; h += (maxHeight / vs)) //outer loop handles height change
		{
			for (THETA = 0; THETA < (2*M_PI); THETA += ((2*M_PI) / rs)) //inner loop handles circles
			{		
				Vertex curPos = Vertex(h*cos(THETA), h, (-h * sin(THETA))); //current position on unit circle

				GLdouble* rotateResult = rotateVertexY(2*M_PI, curPos);
				Vertex rotate = Vertex(rotateResult); //apply rotation matrix
				GLdouble* transformResult = translateVertexY(h, rotate);
				Vertex transform = Vertex(transformResult); //translate up w/ respect to the Y axis via translation matrix

				vertexArray[i].push_back(transform);

				//matrix calculations allocated on heap; MUST be deleted after STL data structure makes hard copy:
				delete[] rotateResult;
				delete[] transformResult;
			}
			i++;
			finalh = h;
		}
		if (i != MAX_VS)
		{
			while (vs >= i) //correction loop
			{
				finalh += (maxHeight / vs);

				Vertex curPos = Vertex(finalh*cos(THETA), finalh, (-finalh * sin(THETA))); //current position on unit circle

				GLdouble* rotateRes = rotateVertexY(2*M_PI, curPos);
				Vertex rotate = Vertex(rotateRes); //apply rotation matrix
				GLdouble* transformRes = translateVertexY(finalh, rotate);
				Vertex transform = Vertex(transformRes); //translate up w/ respect to the Y axis via translation matrix

				vertexArray[i].push_back(transform);

				//matrix calculations allocated on heap; MUST be deleted after STL data structure makes hard copy:
				delete[] rotateRes;
				delete[] transformRes;

				i++;
			}
		}
		//this->translate(0, 5.0, 0);
	}
}

Shape::Shape(Shape *target) //copy constructor
{
	this->vertexArray = target->vertexArray;
}

void Shape::drawCube(int renderMode, bool normalFlag)
{
	for (int i = 0; i < 6; i++)
	{
		quadArray[i][0].draw(renderMode);
	}
}

void Shape::drawCylinder(int renderMode, bool normalFlag)
{
	for (int i = 0; i < vs-1; i++) 
	{
		for (int j = 0; j < rs-1; j++) 
		{
			if ((i!=(vs-1)) && (j!=(rs-1)))
			{
				quadArray[i][j].draw(renderMode);
			}
			if ((i != 0) && (i != (vs-1)) && (j != 0) && ( j != (rs-1)))
			{	
				if (normalFlag == true)
				{
					Vertex v0 = quadArray[i][j].getFP0();
					Vertex v1 = quadArray[i][j].getFP1();
					Vertex v2 = quadArray[i][j].getFP2();
					Vertex v3 = quadArray[i][j].getFP3();

					GLdouble* v0cross = crossProduct3V(v3, v0, v1);
					GLdouble* v0normal = normalize(Vertex(v0cross));

					GLdouble* v1cross = crossProduct3V(v0, v1, v2);
					GLdouble* v1normal = normalize(Vertex(v1cross));

					GLdouble* v2cross = crossProduct3V(v1, v2, v3);
					GLdouble* v2normal = normalize(Vertex(v2cross));

					GLdouble* v3cross = crossProduct3V(v2, v3, v0);
					GLdouble* v3normal = normalize(Vertex(v3cross));

					Vertex v0norm = Vertex(v0normal);
					Vertex v1norm = Vertex(v1normal);
					Vertex v2norm = Vertex(v2normal);
					Vertex v3norm = Vertex(v3normal);

					glColor3f(0.0, 1.0, 0.0);

					/*glBegin(GL_NORMALIZE);
					glNormal3f(v0.x, v0.y, v0.z);
					glNormal3f((v0.x + (.06 * v0norm.x)), (v0.y + (.06 * v0norm.y)), (v0.z + (.06 * v0norm.z)));
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v1.x, v1.y, v1.z);
					glNormal3f((v1.x + (.06 * v1norm.x)), (v1.y + (.06 * v1norm.y)), (v1.z + (.06 * v1norm.z)));
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v2.x, v2.y, v2.z);
					glNormal3f((v2.x + (.06 * v2norm.x)), (v2.y + (.06 * v2norm.y)), (v2.z + (.06 * v2norm.z)));
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v3.x, v3.y, v3.z);
					glNormal3f((v3.x + (.06 * v3norm.x)), (v3.y + (.06 * v3norm.y)), (v3.z + (.06 * v3norm.z)));
					glEnd();*/

					/*
					glBegin(GL_NORMALIZE);
					glNormal3f(v0norm.x, v0norm.y, v0norm.z);
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v1norm.x, v1norm.y, v1norm.z);
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v2norm.x, v2norm.y, v2norm.z);
					glEnd();

					glBegin(GL_NORMALIZE);
					glNormal3f(v3norm.x, v3norm.y, v3norm.z);
					glEnd();
					*/

					Vertex curNorm = Vertex(
						                    ((v0norm.x+v1norm.x+v2norm.x+v3norm.x)/4),
										    ((v0norm.y+v1norm.y+v2norm.y+v3norm.y)/4),
										    ((v0norm.z+v1norm.z+v2norm.z+v3norm.z)/4)
										   );

					GLdouble *normalVector = normalize(curNorm);

					glBegin(GL_NORMALIZE);
					glNormal3f(normalVector[0], normalVector[1], normalVector[2]);
					glEnd();

					delete[] normalVector;

					delete[] v0cross;
					delete[] v0normal;
				
					delete[] v1cross;
					delete[] v1normal;

					delete[] v2cross;
					delete[] v2normal;

					delete[] v3cross;
					delete[] v3normal;

					glColor3fv(this->rgb);
				}
			}
			if (j!=0 && (i==0 || i==(vs-1))) //calculate & draw special case faces for stitching together radial seem
			{
				//glColor3f(1.0, 0.0, 0.0);
				glBegin(renderMode);
				glVertex4f(vertexArray[0][j].x, vertexArray[0][j].y, vertexArray[0][j].z, vertexArray[0][j].w);
				glVertex4f(vertexArray[vs-1][j].x, vertexArray[vs-1][j].y, vertexArray[vs-1][j].z, vertexArray[vs-1][j].w);
				glVertex4f(vertexArray[vs-1][j-1].x, vertexArray[vs-1][j-1].y, vertexArray[vs-1][j-1].z, vertexArray[vs-1][j-1].w);
				glVertex4f(vertexArray[0][j-1].x, vertexArray[0][j-1].y, vertexArray[0][j-1].z, vertexArray[0][j-1].w);
				glEnd();
			}
		}
		if (i!=0) //calculate & draw special case faces for stitching together vertical seem
		{
			glBegin(renderMode);
			glVertex4f(vertexArray[i][0].x, vertexArray[i][0].y, vertexArray[i][0].z, vertexArray[i][0].w);
			glVertex4f(vertexArray[i][rs-1].x, vertexArray[i][rs-1].y, vertexArray[i][rs-1].z, vertexArray[i][rs-1].w);
			glVertex4f(vertexArray[i-1][rs-1].x, vertexArray[i-1][rs-1].y, vertexArray[i-1][rs-1].z, vertexArray[i-1][rs-1].w);
			glVertex4f(vertexArray[i-1][0].x, vertexArray[i-1][0].y, vertexArray[i-1][0].z, vertexArray[i-1][0].w);
			glEnd();
		}
	}
	glBegin(renderMode);
	glVertex4f(vertexArray[0][0].x, vertexArray[0][0].y, vertexArray[0][0].z, vertexArray[0][0].w);
	glVertex4f(vertexArray[0][rs-1].x, vertexArray[0][rs-1].y, vertexArray[0][rs-1].z, vertexArray[0][rs-1].w);
	glVertex4f(vertexArray[vs-1][rs-1].x, vertexArray[vs-1][rs-1].y, vertexArray[vs-1][rs-1].z, vertexArray[vs-1][rs-1].w);
	glVertex4f(vertexArray[vs-1][0].x, vertexArray[vs-1][0].y, vertexArray[vs-1][0].z, vertexArray[vs-1][0].w);
	glEnd();


	//you already have the points needed for triangles. Just figure out where they are.
	int i;
	GLdouble *tempAxis = normalize(curAxis);
	for (i = 0; i < rs-1; i++)
	{
		if (renderMode==GL_LINE_LOOP)
		{ 
			glBegin(GL_LINE_LOOP);
		}
		else 
		{
			glBegin(GL_TRIANGLES);
		}
		glVertex4f(vertexArray[0][i].x, vertexArray[0][i].y, vertexArray[0][i].z, 1.0);
		glVertex4f(vertexArray[0][i+1].x, vertexArray[0][i+1].y, vertexArray[0][i+1].z, 1.0);
		glVertex4f(this->position.x, this->position.y, this->position.z, 1.0);
		glEnd();
	}
	for (i = 0; i < rs-1; i++)
	{
		if (renderMode==GL_LINE_LOOP)
		{ 
			glBegin(GL_LINE_LOOP);
		}
		else 
		{
			glBegin(GL_TRIANGLES);
		}
		glVertex4f(vertexArray[vs-1][i].x, vertexArray[vs-1][i].y, vertexArray[vs-1][i].z, 1.0);
		glVertex4f(vertexArray[vs-1][i+1].x, vertexArray[vs-1][i+1].y, vertexArray[vs-1][i+1].z, 1.0);
		glVertex4f(this->position.x + (shapeHeight * tempAxis[0]), this->position.y + (shapeHeight * tempAxis[1]), this->position.z + (shapeHeight * tempAxis[2]), 1.0);
		glEnd();
	}	
	delete[] tempAxis;
}

void Shape::drawSphere(int renderMode, bool normalFlag)
{
	for (int i = 0; i < vs-1; i++) 
	{
		for (int j = 0; j < rs-1; j++) 
		{
			if ((i!=(vs-1)) && (j!=(rs-1)))
			{
				quadArray[i][j].draw(renderMode);
			}
			if ((i != 0) && (i != vs-1) && (j != 0) && ( j != rs-1))
			{
				//The normal at a vertex should be computed as the normalised sum of all the unit length normals for each face the vertex shares. 
				if (normalFlag==true)
				{
					Vertex v0 = quadArray[i][j].getFP0();
					Vertex v1 = quadArray[i][j].getFP1();
					Vertex v2 = quadArray[i][j].getFP2();
					Vertex v3 = quadArray[i][j].getFP3();

					GLdouble* v0cross = crossProduct3V(v3, v0, v1);
					GLdouble* v0normal = normalize(Vertex(v0cross));

					GLdouble* v1cross = crossProduct3V(v0, v1, v2); 
					GLdouble* v1normal = normalize(Vertex(v1cross));

					GLdouble* v2cross = crossProduct3V(v1, v2, v3);
					GLdouble* v2normal = normalize(Vertex(v2cross));

					GLdouble* v3cross = crossProduct3V(v2, v3, v0);
					GLdouble* v3normal = normalize(Vertex(v3cross));

					Vertex v0norm = Vertex(v0normal);
					Vertex v1norm = Vertex(v1normal);
					Vertex v2norm = Vertex(v2normal);
					Vertex v3norm = Vertex(v3normal);

					glColor3f(0.0, 1.0, 0.0);

					glBegin(GL_LINE_LOOP);
					glVertex3f(v0.x, v0.y, v0.z);
					glVertex3f((v0.x + (.06 * v0norm.x)), (v0.y + (.06 * v0norm.y)), (v0.z + (.06 * v0norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v1.x, v1.y, v1.z);
					glVertex3f((v1.x + (.06 * v1norm.x)), (v1.y + (.06 * v1norm.y)), (v1.z + (.06 * v1norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f((v2.x + (.06 * v2norm.x)), (v2.y + (.06 * v2norm.y)), (v2.z + (.06 * v2norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f((v3.x + (.06 * v3norm.x)), (v3.y + (.06 * v3norm.y)), (v3.z + (.06 * v3norm.z)));
					glEnd();

					delete[] v0cross;
					delete[] v0normal;
				
					delete[] v1cross;
					delete[] v1normal;

					delete[] v2cross;
					delete[] v2normal;

					delete[] v3cross;
					delete[] v3normal;

					glColor3fv(this->rgb);
					
				}
			}
		}
		if (i!=0) //calculate & draw special case faces for stitching together lateral seam
		{
			glBegin(renderMode);
			glVertex4f(vertexArray[i][0].x, vertexArray[i][0].y, vertexArray[i][0].z, vertexArray[i][0].w);
			glVertex4f(vertexArray[i][rs-1].x, vertexArray[i][rs-1].y, vertexArray[i][rs-1].z, vertexArray[i][rs-1].w);
			glVertex4f(vertexArray[i-1][rs-1].x, vertexArray[i-1][rs-1].y, vertexArray[i-1][rs-1].z, vertexArray[i-1][rs-1].w);
			glVertex4f(vertexArray[i-1][0].x, vertexArray[i-1][0].y, vertexArray[i-1][0].z, vertexArray[i-1][0].w);
			glEnd();
		}

	}
}

void Shape::drawTorus(int renderMode, bool normalFlag)
{
	for (int i = 0; i < vs-1; i++) 
	{
		for (int j = 0; j < rs-1; j++) 
		{
			if ((i!=(vs-1)) && (j!=(rs-1)))
			{
				quadArray[i][j].draw(renderMode);
			}
			if ((i != 0) && (i != (vs-1)) && (j != 0) && ( j != (rs-1)))
			{	
				if (normalFlag == true)
				{
					Vertex v0 = quadArray[i][j].getFP0();
					Vertex v1 = quadArray[i][j].getFP1();
					Vertex v2 = quadArray[i][j].getFP2();
					Vertex v3 = quadArray[i][j].getFP3();

					GLdouble* v0cross = crossProduct3V(v3, v0, v1);
					GLdouble* v0normal = normalize(Vertex(v0cross));

					GLdouble* v1cross = crossProduct3V(v0, v1, v2);
					GLdouble* v1normal = normalize(Vertex(v1cross));

					GLdouble* v2cross = crossProduct3V(v1, v2, v3);
					GLdouble* v2normal = normalize(Vertex(v2cross));

					GLdouble* v3cross = crossProduct3V(v2, v3, v0);
					GLdouble* v3normal = normalize(Vertex(v3cross));

					Vertex v0norm = Vertex(v0normal);
					Vertex v1norm = Vertex(v1normal);
					Vertex v2norm = Vertex(v2normal);
					Vertex v3norm = Vertex(v3normal);

					glColor3f(0.0, 1.0, 0.0);

					glBegin(GL_LINE_LOOP);
					glVertex3f(v0.x, v0.y, v0.z);
					glVertex3f((v0.x + (.06 * v0norm.x)), (v0.y + (.06 * v0norm.y)), (v0.z + (.06 * v0norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v1.x, v1.y, v1.z);
					glVertex3f((v1.x + (.06 * v1norm.x)), (v1.y + (.06 * v1norm.y)), (v1.z + (.06 * v1norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f((v2.x + (.06 * v2norm.x)), (v2.y + (.06 * v2norm.y)), (v2.z + (.06 * v2norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f((v3.x + (.06 * v3norm.x)), (v3.y + (.06 * v3norm.y)), (v3.z + (.06 * v3norm.z)));
					glEnd();

					delete[] v0cross;
					delete[] v0normal;
				
					delete[] v1cross;
					delete[] v1normal;

					delete[] v2cross;
					delete[] v2normal;

					delete[] v3cross;
					delete[] v3normal;

					glColor3fv(this->rgb);
				}
			}
			if (j!=0 && (i==0 || i==(vs-1))) //calculate & draw special case faces for stitching together radial seem
			{
				//glColor3f(1.0, 0.0, 0.0);
				glBegin(renderMode);
				glVertex4f(vertexArray[0][j].x, vertexArray[0][j].y, vertexArray[0][j].z, vertexArray[0][j].w);
				glVertex4f(vertexArray[vs-1][j].x, vertexArray[vs-1][j].y, vertexArray[vs-1][j].z, vertexArray[vs-1][j].w);
				glVertex4f(vertexArray[vs-1][j-1].x, vertexArray[vs-1][j-1].y, vertexArray[vs-1][j-1].z, vertexArray[vs-1][j-1].w);
				glVertex4f(vertexArray[0][j-1].x, vertexArray[0][j-1].y, vertexArray[0][j-1].z, vertexArray[0][j-1].w);
				glEnd();
			}
		}
		if (i!=0) //calculate & draw special case faces for stitching together vertical seem
		{
			//glColor3f(1.0, 0.0, 0.0);
			glBegin(renderMode);
			glVertex4f(vertexArray[i][0].x, vertexArray[i][0].y, vertexArray[i][0].z, vertexArray[i][0].w);
			glVertex4f(vertexArray[i][rs-1].x, vertexArray[i][rs-1].y, vertexArray[i][rs-1].z, vertexArray[i][rs-1].w);
			glVertex4f(vertexArray[i-1][rs-1].x, vertexArray[i-1][rs-1].y, vertexArray[i-1][rs-1].z, vertexArray[i-1][rs-1].w);
			glVertex4f(vertexArray[i-1][0].x, vertexArray[i-1][0].y, vertexArray[i-1][0].z, vertexArray[i-1][0].w);
			glEnd();
		}
	}
	//final stitch hack:
	glBegin(renderMode);
	glVertex4f(vertexArray[0][0].x, vertexArray[0][0].y, vertexArray[0][0].z, vertexArray[0][0].w);
	glVertex4f(vertexArray[0][rs-1].x, vertexArray[0][rs-1].y, vertexArray[0][rs-1].z, vertexArray[0][rs-1].w);
	glVertex4f(vertexArray[vs-1][rs-1].x, vertexArray[vs-1][rs-1].y, vertexArray[vs-1][rs-1].z, vertexArray[vs-1][rs-1].w);
	glVertex4f(vertexArray[vs-1][0].x, vertexArray[vs-1][0].y, vertexArray[vs-1][0].z, vertexArray[vs-1][0].w);
	glEnd();
}

void Shape::drawCone(int renderMode, bool normalFlag)
{
	//glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < vs-1; i++) 
	{
		for (int j = 0; j < rs-1; j++) 
		{
			if ((i!=(vs-1)) && (j!=(rs-1)))
			{
				quadArray[i][j].draw(renderMode);
			}
			if ((i != 0) && (i != (vs-1)) && (j != 0) && ( j != (rs-1))) //must exclude boundary cases from vertex normal calculations
			{
				if (normalFlag == true) //if normals enabled, for each non-boundary [i][j]-vertex V in the array, 
				{
					Vertex v0 = quadArray[i][j].getFP0();
					Vertex v1 = quadArray[i][j].getFP1();
					Vertex v2 = quadArray[i][j].getFP2();
					Vertex v3 = quadArray[i][j].getFP3();

					GLdouble* v0cross = crossProduct3V(v3, v0, v1);
					GLdouble* v0normal = normalize(Vertex(v0cross));

					GLdouble* v1cross = crossProduct3V(v0, v1, v2);
					GLdouble* v1normal = normalize(Vertex(v1cross));

					GLdouble* v2cross = crossProduct3V(v1, v2, v3);
					GLdouble* v2normal = normalize(Vertex(v2cross));

					GLdouble* v3cross = crossProduct3V(v2, v3, v0);
					GLdouble* v3normal = normalize(Vertex(v3cross));

					Vertex v0norm = Vertex(v0normal);
					Vertex v1norm = Vertex(v1normal);
					Vertex v2norm = Vertex(v2normal);
					Vertex v3norm = Vertex(v3normal);

					glColor3f(0.0, 1.0, 0.0); //draw normals as green

					glBegin(GL_LINE_LOOP);
					glVertex3f(v0.x, v0.y, v0.z);
					glVertex3f((v0.x + (.06 * v0norm.x)), (v0.y + (.06 * v0norm.y)), (v0.z + (.06 * v0norm.z)));
					//glNormal3f(v0norm.x, v0norm.y, v0norm.z);
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v1.x, v1.y, v1.z);
					glVertex3f((v1.x + (.06 * v1norm.x)), (v1.y + (.06 * v1norm.y)), (v1.z + (.06 * v1norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v2.x, v2.y, v2.z);
					glVertex3f((v2.x + (.06 * v2norm.x)), (v2.y + (.06 * v2norm.y)), (v2.z + (.06 * v2norm.z)));
					glEnd();

					glBegin(GL_LINE_LOOP);
					glVertex3f(v3.x, v3.y, v3.z);
					glVertex3f((v3.x + (.06 * v3norm.x)), (v3.y + (.06 * v3norm.y)), (v3.z + (.06 * v3norm.z)));
					glEnd();

					delete[] v0cross;
					delete[] v0normal;
				
					delete[] v1cross;
					delete[] v1normal;

					delete[] v2cross;
					delete[] v2normal;

					delete[] v3cross;
					delete[] v3normal;

					glColor3fv(this->rgb); //restore shape's base color
				}
			}
			if (j!=0 && (i==0 || i==(vs-1))) //calculate & draw special case faces for stitching together radial seem
			{
				//glColor3f(1.0, 0.0, 0.0);
				glBegin(renderMode);
				glVertex4f(vertexArray[0][j].x, vertexArray[0][j].y, vertexArray[0][j].z, vertexArray[0][j].w);
				glVertex4f(vertexArray[vs-1][j].x, vertexArray[vs-1][j].y, vertexArray[vs-1][j].z, vertexArray[vs-1][j].w);
				glVertex4f(vertexArray[vs-1][j-1].x, vertexArray[vs-1][j-1].y, vertexArray[vs-1][j-1].z, vertexArray[vs-1][j-1].w);
				glVertex4f(vertexArray[0][j-1].x, vertexArray[0][j-1].y, vertexArray[0][j-1].z, vertexArray[0][j-1].w);
				glEnd();
			}
		}
		if (i!=0) //calculate & draw special case faces for stitching together vertical seem
		{
			//glColor3f(1.0, 0.0, 0.0);
			glBegin(renderMode);
			glVertex4f(vertexArray[i][0].x, vertexArray[i][0].y, vertexArray[i][0].z, vertexArray[i][0].w);
			glVertex4f(vertexArray[i][rs-1].x, vertexArray[i][rs-1].y, vertexArray[i][rs-1].z, vertexArray[i][rs-1].w);
			glVertex4f(vertexArray[i-1][rs-1].x, vertexArray[i-1][rs-1].y, vertexArray[i-1][rs-1].z, vertexArray[i-1][rs-1].w);
			glVertex4f(vertexArray[i-1][0].x, vertexArray[i-1][0].y, vertexArray[i-1][0].z, vertexArray[i-1][0].w);
			glEnd();
		}
	}
	//final stitch hack
	glBegin(renderMode);
	glVertex4f(vertexArray[0][0].x, vertexArray[0][0].y, vertexArray[0][0].z, vertexArray[0][0].w);
	glVertex4f(vertexArray[0][rs-1].x, vertexArray[0][rs-1].y, vertexArray[0][rs-1].z, vertexArray[0][rs-1].w);
	glVertex4f(vertexArray[vs-1][rs-1].x, vertexArray[vs-1][rs-1].y, vertexArray[vs-1][rs-1].z, vertexArray[vs-1][rs-1].w);
	glVertex4f(vertexArray[vs-1][0].x, vertexArray[vs-1][0].y, vertexArray[vs-1][0].z, vertexArray[vs-1][0].w);
	glEnd();
}

void Shape::draw(int mode)
{
	glColor3fv(this->rgb);
	bool normalFlag;

	///////////////////////on/off switch for normals/////////////////
	if (this->getType()==SHAPE_CYLINDER) normalFlag = true;
	else normalFlag = false;
	/////////////////////////////////////////////////////////////////

	if (type==0) drawCube(mode, normalFlag);
	else if (type==1) drawCylinder(mode, normalFlag);
	else if (type==2) drawSphere(mode, normalFlag);
	else if (type==3) drawTorus(mode, normalFlag);
	else if (type==4) drawCone(mode, normalFlag);
}

void Shape::tessellate() //handles tessellation of the objects
{
	if (this->type!=SHAPE_CUBE) //calculate primary faces
	{
		for (int i = 0; i < vs; i++) //Vertical Stacks
		{
			for (int j = 0; j < rs; j++) //Radial Slices
			{
				if ((i!=(vs-1)) && (j!=(rs-1)))
				{
					quadArray[i].push_back(
										   Face(
												&vertexArray[i][j], 
												&vertexArray[i][j+1], 
												&vertexArray[i+1][j+1], 
												&vertexArray[i+1][j]
											   )
										  );
				}
			}
		}
	}
	else //cube
	{
		quadArray[0].push_back(Face(&vertexArray[0][0], &vertexArray[0][1], &vertexArray[1][1], &vertexArray[1][0]));
		quadArray[1].push_back(Face(&vertexArray[1][0], &vertexArray[1][1], &vertexArray[2][1], &vertexArray[2][0]));
		quadArray[2].push_back(Face(&vertexArray[2][0], &vertexArray[2][1], &vertexArray[3][1], &vertexArray[3][0]));
		quadArray[3].push_back(Face(&vertexArray[3][0], &vertexArray[3][1], &vertexArray[0][1], &vertexArray[0][0]));
		quadArray[4].push_back(Face(&vertexArray[3][0], &vertexArray[0][0], &vertexArray[1][0], &vertexArray[2][0]));
		quadArray[5].push_back(Face(&vertexArray[3][1], &vertexArray[0][1], &vertexArray[1][1], &vertexArray[2][1]));
	}
}

void Shape::translate(GLfloat x, GLfloat y, GLfloat z) //translate shape by {x, y, z}
{
	//printf("Translating shape by {%g, %g, %g}\n", x, y, z);
	for (int i = 0; i < vs; i++)
	{
		for (int j = 0; j < rs; j++)
		{
			GLdouble* temp = translateVertex(vertexArray[i][j], x, y, z);
			Vertex target = Vertex(temp);

			vertexArray[i][j].x = target.x;
			vertexArray[i][j].y = target.y;
			vertexArray[i][j].z = target.z;
			vertexArray[i][j].w = target.w;

			delete[] temp;	//result of matrix multiplication is on heap
		}
	}

	/////////////////////////MODIFY CURRENT CTM///////////////
	GLdouble T[4][4] = {
						{1.0, 0.0, 0.0,  x},
						{0.0, 1.0, 0.0,  y},
						{0.0, 0.0, 1.0,  z},
						{0.0, 0.0, 0.0, dw},
					   };

	GLdouble **tempCTM = matrixMult(CTM, T);

	GLdouble Tinverse[4][4] = {
						       {1.0, 0.0, 0.0,  -x},
						       {0.0, 1.0, 0.0,  -y},
						       {0.0, 0.0, 1.0,  -z},
						       {0.0, 0.0, 0.0,  dw},
					          };	

	GLdouble **tempCTMinverse = matrixMult(CTMinverse, Tinverse);
	//this->CTM = tempCTM;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CTM[i][j] = tempCTM[i][j];
			CTMinverse[i][j] = tempCTMinverse[i][j];
		}
	}
	//////////////////////////////////////////////////////////	

	GLdouble* tempPos = translateVertex(position, x, y, z);
	this->position = Vertex(tempPos);
	delete[] tempPos;
	delete[] tempCTMinverse;
}

void Shape::translateShapeLocalized(GLfloat amt, int x, int y, int z) //move shape by amt in x/y/z dir
{

}

void Shape::rotate(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) //x/y/z are flags denoting the rotational direction
{
	char axis = '0';
	if (x!=0) axis = 'X';
	else if (y!=0) axis = 'Y';
	else if (z!=0) axis = 'Z';
	printf("rotating %g degrees about the %c axis\n", deg, axis);

	for (int i = 0; i < vs; i++)
	{
		for (int j = 0; j < rs; j++)
		{
			GLdouble* temp = rotateVertex(vertexArray[i][j], deg, x, y, z);
			Vertex target = Vertex(temp);

			vertexArray[i][j].x = target.x;
			vertexArray[i][j].y = target.y;
			vertexArray[i][j].z = target.z;
			vertexArray[i][j].w = target.w;

			delete[] temp;	//result of matrix multiplication is on heap
		}
	}

	/////////////////////////MODIFY CURRENT CTM///////////////
	GLdouble THETA = deg;
	GLdouble **tempCTM;
	GLdouble **tempCTMinverse;
	GLdouble **tempCTMdir_inverse;

	if (x==1 || x==-1)
	{
		GLdouble rotateX[4][4] = {
								  {1.0, 0.0,		  0.0,	             0.0}, 
								  {0.0, cos(x*THETA), (-1*sin(x*THETA)), 0.0}, 
								  {0.0, sin(x*THETA), cos(x*THETA),      0.0}, 
								  {0.0, 0.0,	      0.0,	             1.0}, 
								 };

		tempCTM = matrixMult(this->CTM, rotateX);
		
		THETA*=-1;
		GLdouble rotateX_inverse[4][4] = {
								          {1.0, 0.0,		  0.0,	             0.0}, 
								          {0.0, cos(x*THETA), (-1*sin(x*THETA)), 0.0}, 
								          {0.0, sin(x*THETA), cos(x*THETA),      0.0}, 
								          {0.0, 0.0,	      0.0,	             1.0}, 
								         };

        tempCTMinverse = matrixMult(this->CTMinverse, rotateX_inverse);
        tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, rotateX_inverse);
	}
	else if (y==1 || y==-1)
	{
		GLdouble rotateY[4][4] = {
								  {cos(y*THETA),       0.0, sin(y*THETA), 0.0}, 
							      {0.0,                1.0, 0.0,          0.0}, 
							      {(-1*sin(y*THETA)),  0.0, cos(y*THETA), 0.0}, 
							      {0.0,                0.0, 0.0,          1.0}, 
						         };

		tempCTM = matrixMult(this->CTM, rotateY);

		THETA*=-1;
		GLdouble rotateY_inverse[4][4] = {
								          {cos(y*THETA),       0.0, sin(y*THETA), 0.0}, 
							              {0.0,                1.0, 0.0,          0.0}, 
							              {(-1*sin(y*THETA)),  0.0, cos(y*THETA), 0.0}, 
							              {0.0,                0.0, 0.0,          1.0}, 
						                 };

		tempCTMinverse = matrixMult(this->CTMinverse, rotateY_inverse);
		tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, rotateY_inverse);
	}
	else if (z==1 || z==-1)
	{
		GLdouble rotateZ[4][4] = {
								  {cos(z*THETA), (-1*sin(z*THETA)), 0.0, 0.0}, 
							      {sin(z*THETA), cos(z*THETA),      0.0, 0.0}, 
							      {0.0,			 0.0,               1.0, 0.0}, 
							      {0.0,			 0.0,               0.0, 1.0}, 
						         };

		tempCTM = matrixMult(this->CTM, rotateZ);

		THETA*=-1;
		GLdouble rotateZ_inverse[4][4] = {
								          {cos(z*THETA), (-1*sin(z*THETA)), 0.0, 0.0}, 
							              {sin(z*THETA), cos(z*THETA),      0.0, 0.0}, 
							              {0.0,			 0.0,               1.0, 0.0}, 
							              {0.0,			 0.0,               0.0, 1.0}, 
						                 };		

		tempCTMinverse = matrixMult(this->CTMinverse, rotateZ_inverse);
		tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, rotateZ_inverse);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CTM[i][j] = tempCTM[i][j];
			CTMinverse[i][j] = tempCTMinverse[i][j];
			CTMdir_inverse[i][j] = tempCTMdir_inverse[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] tempCTM[i];
		delete[] tempCTMinverse[i];
		delete[] tempCTMdir_inverse[i];
	}
	delete[] tempCTM;
	delete[] tempCTMinverse;
	delete[] tempCTMdir_inverse;
	//////////////////////////////////////////////////////////		
	
	//ensure matrix is also applied to shape main axes:
	GLdouble* axisRot = rotateVertex(curAxis, deg, x, y, z);
	curAxis = Vertex(axisRot);
	delete[] axisRot;

	GLdouble* XaxisRot = rotateVertex(curXAxis, deg, x, y, z);
	curXAxis = Vertex(XaxisRot);
	delete[] XaxisRot;

	GLdouble* YaxisRot = rotateVertex(curZAxis, deg, x, y, z);
	curZAxis = Vertex(YaxisRot);
	delete[] YaxisRot;

	GLdouble* tempPos = rotateVertex(position, deg, x, y, z);
	position = Vertex(tempPos);
	delete[] tempPos;
}

void Shape::revolve(GLfloat deg, GLfloat x, GLfloat y, GLfloat z)
{
	//SAVE POSITION VARIABLES
	GLdouble xval = position.x;
	GLdouble yval = position.y;
	GLdouble zval = position.z;
	Vertex tempPos = Vertex(xval, yval, zval);

	if (this->getType()==SHAPE_CONE)printf("Revolving...x-axis = {%f, %f, %f}\n", curXAxis.x, curXAxis.y, curXAxis.z);
	translate((-1*position.x), (-1*position.y), (-1*position.z)); //Translate object to the origin via position negated

	for (int i = 0; i < vs; i++)
	{
		for (int j = 0; j < rs; j++)
		{
			GLdouble* temp = rotateVertexLocalized(vertexArray[i][j], deg, curXAxis.x, curXAxis.y, curXAxis.z);
			Vertex target = Vertex(temp);

			vertexArray[i][j].x = target.x;
			vertexArray[i][j].y = target.y;
			vertexArray[i][j].z = target.z;
			vertexArray[i][j].w = target.w;

			delete[] temp;	//result of matrix multiplication is on heap
		}
	}

	/////////////////////////MODIFY CURRENT CTM///////////////
	x = this->curXAxis.x;
	y = this->curXAxis.y;
	z = this->curXAxis.z;
	GLdouble THETA = deg;

	GLdouble M[4][4] = {
	                    {(pow(x, 2)+(cos(THETA)*(1-pow(x, 2)))),	 (x*(y*(1-cos(THETA)))-(z*sin(THETA))),	   (z*(x*(1-cos(THETA)))+(y*sin(THETA))),   0.0},
	                    {(x*(y*(1-cos(THETA)))+(z*sin(THETA))),      (pow(y, 2)+(cos(THETA)*(1-pow(y, 2)))),   (y*(z*(1-cos(THETA)))-(x*sin(THETA))),   0.0},
	                    {(z*(x*(1-cos(THETA)))-(y * sin(THETA))),	 (y*(z*(1-cos(THETA)))+(x*sin(THETA))),	   (pow(z, 2)+(cos(THETA)*(1-pow(z, 2)))),  0.0}, 
	                    {0.0,									     0.0,									   0.0,                                     1.0}, 
	                   };

	GLdouble **tempCTM = matrixMult(this->CTM, M);
	//this->CTM = tempCTM;

	THETA*=-1; //make deg negative for inverse
	GLdouble Minverse[4][4] = {
	                           {(pow(x, 2)+(cos(THETA)*(1-pow(x, 2)))),	    (x*(y*(1-cos(THETA)))-(z*sin(THETA))),	   (z*(x*(1-cos(THETA)))+(y*sin(THETA))),   0.0},
	                           {(x*(y*(1-cos(THETA)))+(z*sin(THETA))),      (pow(y, 2)+(cos(THETA)*(1-pow(y, 2)))),    (y*(z*(1-cos(THETA)))-(x*sin(THETA))),   0.0},
	                           {(z*(x*(1-cos(THETA)))-(y * sin(THETA))),	(y*(z*(1-cos(THETA)))+(x*sin(THETA))),	   (pow(z, 2)+(cos(THETA)*(1-pow(z, 2)))),  0.0}, 
	                           {0.0,									    0.0,									   0.0,                                     1.0}, 
	                          };

	GLdouble **tempCTMinverse = matrixMult(this->CTMinverse, Minverse);
	GLdouble **tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, Minverse);
	//this->CTMinverse = tempCTMinverse;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CTM[i][j] = tempCTM[i][j];
			CTMinverse[i][j] = tempCTMinverse[i][j];
			CTMdir_inverse[i][j] = tempCTMdir_inverse[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] tempCTM[i];
		delete[] tempCTMinverse[i];
		delete[] tempCTMdir_inverse[i];
	}
	delete[] tempCTM;
	delete[] tempCTMinverse;
	delete[] tempCTMdir_inverse;
	//////////////////////////////////////////////////////////	

	///////////////////////////////////////////////////Keep track of local axes///////////////////////////////////////////////////
	//GLdouble* tempLoc = rotateVertexLocalized(position, deg, curAxis.x, curAxis.y, curAxis.z);
	//position = Vertex(tempLoc);
	//delete[] tempLoc;

	GLdouble* axisRot = rotateVertexLocalized(curAxis, deg, curXAxis.x, curXAxis.y, curXAxis.z);
	curAxis = Vertex(axisRot);
	delete[] axisRot;

	GLdouble* XaxisRot = rotateVertexLocalized(curXAxis, deg, curXAxis.x, curXAxis.y, curXAxis.z);
	curXAxis = Vertex(XaxisRot);
	delete[] XaxisRot;

	GLdouble* ZaxisRot = rotateVertexLocalized(curZAxis, deg, curXAxis.x, curXAxis.y, curXAxis.z);
	curZAxis = Vertex(ZaxisRot);
	delete[] ZaxisRot;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	translate(tempPos.x, tempPos.y, tempPos.z);
}

void Shape::scale(GLfloat x, GLfloat y, GLfloat z)
{
	//printf("entering Scale\n");
	for (int i = 0; i < vs; i++)
	{
		for (int j = 0; j < rs; j++)
		{
			GLdouble* temp = scaleVertex(vertexArray[i][j], x, y, z);
			Vertex target = Vertex(temp);

			vertexArray[i][j].x = target.x;
			vertexArray[i][j].y = target.y;
			vertexArray[i][j].z = target.z;
			vertexArray[i][j].w = target.w;

			delete[] temp;	//result of matrix multiplication is on heap
		}
	}

	/////////////////////////MODIFY CURRENT CTM///////////////
	GLdouble S[4][4] = {
						{(x), 0.0,   0.0,    0.0},
						{0.0,   (y), 0.0,    0.0},
						{0.0,   0.0,   (z),  0.0},
						{0.0,   0.0,   0.0,  1.0},
					   };

	GLdouble Sinverse[4][4] = {
						       {((1/x)), 0.0,        0.0,         0.0},
						       {0.0,        ((1/y)), 0.0,         0.0},
						       {0.0,        0.0,        ((1/z)),  0.0},
						       {0.0,        0.0,        0.0,      1.0},
					          };

	GLdouble **tempCTM = matrixMult(this->CTM, S);
	//this->CTM = tempCTM;

	GLdouble **tempCTMinverse = matrixMult(this->CTMinverse, Sinverse);
	//this->CTMinverse = tempCTMinverse;

	GLdouble **tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, Sinverse);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CTM[i][j] = tempCTM[i][j];
			CTMinverse[i][j] = tempCTMinverse[i][j];
			CTMdir_inverse[i][j] = tempCTMdir_inverse[i][j];
		}
	}	

	for (int i = 0; i < 4; i++)
	{
		delete[] tempCTM[i];
		delete[] tempCTMinverse[i];
		delete[] tempCTMdir_inverse[i];
	}
	delete[] tempCTM;
	delete[] tempCTMinverse;
	delete[] tempCTMdir_inverse;
	//////////////////////////////////////////////////////////	

	//ensure matrix is also applied to shape main axis:
	GLdouble* tempPos = scaleVertex(position, x, y, z);
	position = Vertex(tempPos);
	delete[] tempPos;
}

void Shape::arbitraryRotate(GLdouble deg, GLdouble ax, GLdouble ay, GLdouble az, GLdouble cx, GLdouble cy, GLdouble cz)
{
	//normalize target axis:
	GLdouble* tempNorm = normalize(Vertex(ax, ay, az));
	Vertex normalizedAxis = Vertex(tempNorm);
	delete[] tempNorm;

	//translate shape to axis:
	translate((-1*cx), (-1*cy), (-1*cz));

	//apply rodrigues formula:
	for (int i = 0; i < vs; i++)
	{
		for (int j = 0; j < rs; j++)
		{
			GLdouble* tempRotate = rotateVertexLocalized(
				                                         vertexArray[i][j],
			                                             deg,
			                                             normalizedAxis.x, 
			                                             normalizedAxis.y, 
			                                             normalizedAxis.z
			                                            );
			Vertex target = Vertex(tempRotate);

			vertexArray[i][j].x = target.x;
			vertexArray[i][j].y = target.y;
			vertexArray[i][j].z = target.z;
			vertexArray[i][j].w = target.w;

			delete[] tempRotate; //deallocate 
		}
	}

	/////////////////////////MODIFY CURRENT CTM///////////////
	GLdouble x = normalizedAxis.x;
	GLdouble y = normalizedAxis.y;
	GLdouble z = normalizedAxis.z;
	GLdouble THETA = deg;
	
	GLdouble M[4][4] = {
	                    {(pow(x, 2)+(cos(THETA)*(1-pow(x, 2)))),	 (x*(y*(1-cos(THETA)))-(z*sin(THETA))),	   (z*(x*(1-cos(THETA)))+(y*sin(THETA))),   0.0},
	                    {(x*(y*(1-cos(THETA)))+(z*sin(THETA))),      (pow(y, 2)+(cos(THETA)*(1-pow(y, 2)))),   (y*(z*(1-cos(THETA)))-(x*sin(THETA))),   0.0},
	                    {(z*(x*(1-cos(THETA)))-(y * sin(THETA))),	 (y*(z*(1-cos(THETA)))+(x*sin(THETA))),	   (pow(z, 2)+(cos(THETA)*(1-pow(z, 2)))),  0.0}, 
	                    {0.0,									     0.0,									   0.0,                                     1.0}, 
	                   };

	GLdouble **tempCTM = matrixMult(this->CTM, M);

	THETA*=-1; 
	GLdouble Minverse [4][4] = {
	                            {(pow(x, 2)+(cos(THETA)*(1-pow(x, 2)))),	 (x*(y*(1-cos(THETA)))-(z*sin(THETA))),	   (z*(x*(1-cos(THETA)))+(y*sin(THETA))),   0.0},
	                            {(x*(y*(1-cos(THETA)))+(z*sin(THETA))),      (pow(y, 2)+(cos(THETA)*(1-pow(y, 2)))),   (y*(z*(1-cos(THETA)))-(x*sin(THETA))),   0.0},
	                            {(z*(x*(1-cos(THETA)))-(y * sin(THETA))),	 (y*(z*(1-cos(THETA)))+(x*sin(THETA))),	   (pow(z, 2)+(cos(THETA)*(1-pow(z, 2)))),  0.0}, 
	                            {0.0,									     0.0,									   0.0,                                     1.0}, 
	                           };

	GLdouble **tempCTMinverse = matrixMult(this->CTMinverse, Minverse);
	GLdouble **tempCTMdir_inverse = matrixMult(this->CTMdir_inverse, Minverse);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->CTM[i][j] = tempCTM[i][j];
			this->CTMinverse[i][j] = tempCTMinverse[i][j];
			this->CTMdir_inverse[i][j] = tempCTMdir_inverse[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		delete[] tempCTM[i];
		delete[] tempCTMinverse[i];
		delete[] tempCTMdir_inverse[i];
	}
	delete[] tempCTM;
	delete[] tempCTMinverse;
	delete[] tempCTMdir_inverse;
	//////////////////////////////////////////////////////////		

	GLdouble* tempPos = rotateVertexLocalized(position, deg, normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);
	position = Vertex(tempPos);
	delete[] tempPos;

	GLdouble* axisRot = rotateVertexLocalized(curAxis, deg, normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);
	curAxis = Vertex(axisRot);
	delete[] axisRot;

	GLdouble* XaxisRot = rotateVertexLocalized(curXAxis, deg, normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);
	curXAxis = Vertex(XaxisRot);
	delete[] XaxisRot;

	GLdouble* YaxisRot = rotateVertexLocalized(curZAxis, deg, normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);
	curZAxis = Vertex(YaxisRot);
	delete[] YaxisRot;

	translate(cx, cy, cz);
}

//if a line originating at Peye and travelling in direction 'Pworld' intersects with this shape, return the 3d point of intersection
GLdouble* Shape::isHit(Vertex Peye, Vertex dir) //if this shape is in the way of the passed-in ray return true; else return false
{
	//////////////////////Transform Ray Elements by CTM///////////////////////////
	GLdouble vecPeye[3] = {Peye.x, Peye.y, Peye.z};
	GLdouble vecDir[3] = {dir.x, dir.y, dir.z};
	GLdouble *Peye_ctm = vectorMatrixMult(vecPeye, this->CTMinverse); //apply inverse ctm to Peye
	GLdouble *dir_ctm = vectorMatrixMult(vecDir, this->CTMdir_inverse); //apply inverse direction ctm to direction
	//////////////////////////////////////////////////////////////////////////////	

	Peye = Vertex(Peye_ctm);
	dir = Vertex(dir_ctm);

	printf("Peye = {%g, %g, %g}\n", Peye.x, Peye.y, Peye.z);
	printf("Dir = {%g, %g, %g}\n}\n", dir.x, dir.y, dir.z);

	GLdouble *ret;
	GLdouble a, b, c;
	GLdouble *result;

	GLdouble hitX0, hitY0, hitZ0, hitX1, hitY1, hitZ1;

	if (this->getType()==SHAPE_CONE) //calculate intersection of cone
	{
		//implement implicit equation for a cone
		ret=new GLdouble[7];
	}
	else if (this->getType()==SHAPE_CYLINDER)
	{
		//ROTATE the RAY by the INVERSE matrix of however youd rotate the cylinder to get it rightside up
		GLdouble THETA = -8.0;
		GLdouble rotateX[4][4] = {
								  {1.0, 0.0,		0.0,	         0.0}, 
								  {0.0, cos(THETA), (-1*sin(THETA)), 0.0}, 
								  {0.0, sin(THETA), cos(THETA),      0.0}, 
								  {0.0, 0.0,	    0.0,	         1.0}, 
								 };		

		GLdouble *dir_rot = vectorMatrixMult(dir, rotateX);
		GLdouble *Peye_rot = vectorMatrixMult(Peye, rotateX);
		
		dir.x = dir_rot[0];
		dir.y = dir_rot[1];
		dir.z = dir_rot[2];

		Peye.x = Peye_rot[0];
		Peye.y = Peye_rot[1];
		Peye.z = Peye_rot[2];

		delete[] dir_rot;
		delete[] Peye_rot;

		//implement implicit equation for a cylinder
		//GLdouble *result;
		//GLdouble a, b, c;
		a = (pow(dir.x, 2) + pow(dir.y, 2));
		b = (2 * ((Peye.x * dir.x) + (Peye.y * dir.y)));
		c = ((pow(Peye.x, 2) + pow(Peye.y, 2))-1);
		result = getZeroes(a, b, c); //quadratic formula

		if (result[2]==1) //values from getZeroes() are valid, but only one intersection reported.
		{	
			ret = new GLdouble[3];

			//implement equation for a ray: r(t) = p + td
			hitX0 = Peye.x + (dir.x * result[0]);
			hitY0 = Peye.y + (dir.y * result[0]);
			hitZ0 = Peye.z + (dir.z * result[0]);
			
			ret[0]=hitX0;
			ret[1]=hitY0;
			ret[2]=hitZ0; //only one intersection reported.

			ret[3]=0.0;
			ret[4]=0.0;
			ret[5]=0.0;

			ret[6]=1;

			printf("********************\nCYL: TANGENTIAL HIT DETECTED! Zero: . . . . . {%g}\n*********************\n", result[0]);

			delete[] result;
		}
		else if (result[2]==2) //values from getZeroes() are valid, two intersections reported.
		{
			ret = new GLdouble[7];

			//implement equation for a ray: r(t) = p + td
			hitX0 = Peye.x + (dir.x * result[0]);
			hitY0 = Peye.y + (dir.y * result[0]);
			hitZ0 = Peye.z + (dir.z * result[0]);

			hitX1 = Peye.x + (dir.x * result[1]);
			hitY1 = Peye.y + (dir.y * result[1]);
			hitZ1 = Peye.z + (dir.z * result[1]);

			ret[0] = hitX0;
			ret[1] = hitY0;
			ret[2] = hitZ0;

			ret[3] = hitX1;
			ret[4] = hitY1;
			ret[5] = hitZ1;

			ret[6] = 2;

			printf("************************\nCYL: TWO HITS RECORDED! Zeroes: . . . . . {%g, %g}\n***********************\n", result[0], result[1]);

			delete[] result;
		}
		else 
		{
			result = new GLdouble[3];
			//printf("NO HITS RECORDED. a = %g, b = %g, c = %g\n", a, b, c);
			printf("no hits recorded. \n");
			ret = new GLdouble[7];
			delete[] result;
		}	
	}
	else if (this->getType()==SHAPE_CUBE)
	{
		result = new GLdouble[3];
		ret = new GLdouble[7];
		//implement implicit equation for a cube
		//printf("CUBE!\n");
		GLdouble zeroVal = 0;
		GLdouble cubeHits[3];
		
		//Cube = 6 individual 3d planes defined as follows:
		if (dir.x != 0) //LEFT/RIGHT
		{
			//LEFT//////////////////////////////////////////////
			zeroVal = (-0.5 - Peye.x) / dir.x;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;

			if (cubeHits[1] <= 0.5 && cubeHits[2] <= 0.5 && cubeHits[2] >= -0.5) //if hit is in bounds
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////

			//RIGHT/////////////////////////////////////////////
			zeroVal = (0.5 - Peye.x) / dir.x;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;			

			if (cubeHits[1] <= 0.5 && cubeHits[1] >= -0.5 && cubeHits[2] <= 0.5 && cubeHits[2] >= -0.5)
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////
		}
		if (dir.y != 0) //BOTTOM/TOP
		{
			//TOP///////////////////////////////////////////////
			zeroVal = (-0.5 - Peye.y) / dir.y;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;

			if (cubeHits[0] <= 0.5 && cubeHits[0] >= -0.5 && cubeHits[2] <= 0.5 && cubeHits[2] >= -0.5)
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////

			//BOTTOM/////////////////////////////////////////////
			zeroVal = (0.5 - Peye.y) / dir.y;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;

			if (cubeHits[0] <= 0.5 && cubeHits[0] >= -0.5 && cubeHits[2] <= 0.5 && cubeHits[2] >= -0.5)
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////			
		}
		if (dir.z != 0) //FRONT/BACK
		{
			//FRONT//////////////////////////////////////////////
			zeroVal = (-0.5 - Peye.z) / dir.z;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;

			if (cubeHits[1] <= 0.5 && cubeHits[1] >= -0.5 && cubeHits[0] <= 0.5 && cubeHits[0] >= -0.5)
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////

			//BACK//////////////////////////////////////////////
			zeroVal = (0.5 - Peye.z) / dir.z;

			cubeHits[0] = dir.x*zeroVal;
			cubeHits[1] = dir.y*zeroVal;
			cubeHits[2] = dir.z*zeroVal;

			cubeHits[0] += Peye.x;
			cubeHits[1] += Peye.y;
			cubeHits[2] += Peye.z;

			if (cubeHits[1] <= 0.5 && cubeHits[1] >= -0.5 && cubeHits[0] <= 0.5 && cubeHits[0] >= -0.5)
			{
				if (result[0] != 2)
				{
					result[0] += 1;
					int temp = result[0];
					result[temp] = zeroVal;
				}
			}
			////////////////////////////////////////////////////	
		}

		hitX0 = Peye.x + (dir.x * result[1]);
		hitY0 = Peye.y + (dir.y * result[1]);
		hitZ0 = Peye.z + (dir.z * result[1]);

		hitX1 = Peye.x + (dir.x * result[2]);
		hitY1 = Peye.y + (dir.y * result[2]);
		hitZ1 = Peye.z + (dir.z * result[2]);

		Vertex *hitA = new Vertex(hitX0, hitY0, hitZ0);
		Vertex *hitB = new Vertex(hitX1, hitY1, hitZ1);

		ret[6]=0;
		if (hitA->x <= 0.5 && hitA->x >= -0.5)
		{
			if (hitA->y <= 0.5 && hitA->y >= -0.5)
			{
				if (hitA->z <= 0.5 && hitA->z >= -0.5)
				{
					ret[6]++;
					printf("Hit reported at {%g, %g, %g}\n", hitA->x, hitA->y, hitA->z);
				}
			}
		}
		if (hitB->x <= 0.5 && hitB->x >= -0.5)
		{
			if (hitB->y <= 0.5 && hitB->y >= -0.5)
			{
				if (hitB->z <= 0.5 && hitB->z >= -0.5)
				{
					ret[6]++;
					printf("Hit reported at {%g, %g, %g}\n", hitB->x, hitB->y, hitB->z);
				}
			}
		}		

		ret[0] = hitX0;
		ret[1] = hitY0;
		ret[2] = hitZ0;

		ret[3] = hitX1;
		ret[4] = hitY1;
		ret[5] = hitZ1;

		//ret[6] = 2;

		delete hitA;
		delete hitB;
		delete[] result;

		//ret=NULL;
	}
	else if (this->getType()==SHAPE_SPHERE) //implement implicit equation for a sphere, test for intersection w/ ray
	{
		printf("Target Geometry: . . . . . SPHERE\n");
		a = (pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2));
		b = (2*((Peye.x * dir.x) + (Peye.y * dir.y) + (Peye.z * dir.z)));
		c = (pow(Peye.x, 2) + pow(Peye.y, 2) + pow(Peye.z, 2))-1;
		result = getZeroes(a, b, c);

		//NOW: plug resultant values into x=p+dt based on value of flag
		if (result[2]==1) //values from getZeroes() are valid, but only one intersection reported.
		{	
			ret = new GLdouble[3];

			//implement equation for a ray: r(t) = p + td
			hitX0 = Peye.x + (dir.x * result[0]);
			hitY0 = Peye.y + (dir.y * result[0]);
			hitZ0 = Peye.z + (dir.z * result[0]);
			
			ret[0]=hitX0;
			ret[1]=hitY0;
			ret[2]=hitZ0; //only one intersection reported.

			ret[3]=0.0;
			ret[4]=0.0;
			ret[5]=0.0;

			ret[6]=1;

			printf("********************\nTANGENTIAL HIT DETECTED! Zero: . . . . . {%g}\n*********************\n", result[0]);

			delete[] result;
		}
		else if (result[2]==2) //values from getZeroes() are valid, two intersections reported.
		{
			ret = new GLdouble[7];

			//implement equation for a ray: r(t) = p + td
			hitX0 = Peye.x + (dir.x * result[0]);
			hitY0 = Peye.y + (dir.y * result[0]);
			hitZ0 = Peye.z + (dir.z * result[0]);

			hitX1 = Peye.x + (dir.x * result[1]);
			hitY1 = Peye.y + (dir.y * result[1]);
			hitZ1 = Peye.z + (dir.z * result[1]);

			ret[0] = hitX0;
			ret[1] = hitY0;
			ret[2] = hitZ0;

			ret[3] = hitX1;
			ret[4] = hitY1;
			ret[5] = hitZ1;

			ret[6] = 2;

			printf("************************\nTWO HITS RECORDED! Zeroes: . . . . . {%g, %g}\n***********************\n", result[0], result[1]);

			delete[] result;
		}
		else 
		{
			//printf("NO HITS RECORDED. a = %g, b = %g, c = %g\n", a, b, c);
			printf("no hits recorded. \n");
			ret = new GLdouble[7];
			delete[] result;
		}
	}
	else if (this->getType()==SHAPE_TORUS)
	{
		//implement implicit equation for a torus
		ret=new GLdouble[7];
	}

	delete[] Peye_ctm;
	delete[] dir_ctm;
	
	return ret; //check for NULL!!!
}

void Shape::jump()
{
	if (this->position.y<.5)
	{
		dy*=-1.0; //reverse velocity when ground is hit.	
	} 

	dy+=0.5; //apply resistance.
	this->translate(0.0, -dy, 0.0); //update y pos

	if (this->position.y < 0.0) 
	{
		this->translate(0.0, (-1*this->position.y), 0.0); //correct any negative values resulting from incrementing past zero.
	}
	//if (this->position.y!=0 && (dy!=0.5 || dy!=0)) printf("y = %g dy = %g\n", this->position.y, dy);
	if ((this->position.y==0) && (abs(dy)<=4)) 
	{
		rest=true; //if the object settles on Y=0 w/ an increment less than 10, halt further updates. The ball/rectangle is at rest.
	}
}

/*
Vertex* Shape::getPosition()
{
	return &(this->position);
}
*/

GLfloat *Shape::getPosition()
{
	GLfloat *ret = new GLfloat[3];
	ret[0] = this->position.x;
	ret[1] = this->position.y;
	ret[2] = this->position.z;

	return ret;
}

void Shape::paintXAxis()
{
	GLdouble *temp = normalize(curXAxis);

	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	//printf("drawing ray between {%g, %g, %g} and {%g, %g, %g}\n", position.x, position.y, position.z, (temp[0] + (200*temp[0])), (temp[1] + (200*temp[1])), (temp[2] + (200*temp[2])));
	glVertex3f(this->position.x, (this->position.y), this->position.z);
	glVertex3f((this->position.x + (1000*temp[0])), ((this->position.y) + (1000*temp[1])), (this->position.z + (1000*temp[2])));
	glEnd();

	delete[] temp;
}

bool* Shape::fireRay(std::deque<Shape> *targetList, std::deque<Shape> *indicatorList)
{
	GLdouble *intersectResult;
	this->paintXAxis(); //draw line representing ray 

	bool *hitList = new bool[targetList->size()]; //return value; an array of booleans indicating which shapes, if any, are in the firing arc of the ray.
	for (int i = 0; i < targetList->size(); i++)
	{
		hitList[i] = false;
	}

	int index = 0;
	for(std::deque<Shape>::iterator iter = targetList->begin(); iter != targetList->end(); iter++)
	{
		intersectResult = iter->isHit(this->position, this->curXAxis);
		if (intersectResult!=NULL) //check for null return...
		{
			hitList[index]=true;
			if (intersectResult[6]==2)
			{
				printf("calculating intersect indicators... at {%g, %g, %g} and {%g, %g, %g}\n", intersectResult[0],
				                                                                                 intersectResult[1],
				                                                                                 intersectResult[2],
				                                                                                 intersectResult[3],
				                                                                                 intersectResult[4], 
				                                                                                 intersectResult[5]
				      );
	
				indicatorList->push_front(Shape(SHAPE_SPHERE, 10.0, 10.0, 1.0, targetColor));
				indicatorList->front().tessellate();
				indicatorList->front().translate(intersectResult[0], intersectResult[1], intersectResult[2]);
				indicatorList->front().scale(.55, .55, .55);

				indicatorList->push_front(Shape(SHAPE_SPHERE, 10.0, 10.0, 1.0, targetColor));
				indicatorList->front().tessellate();
				indicatorList->front().translate(intersectResult[3], intersectResult[4], intersectResult[5]);
				indicatorList->front().scale(.55, .55, .55);
			}
			else if (intersectResult[6]==1)
			{
				indicatorList->push_front(Shape(SHAPE_SPHERE, 10.0, 10.0, 1.0, targetColor));
				indicatorList->front().tessellate();
				indicatorList->front().translate(intersectResult[0], intersectResult[1], intersectResult[2]);				
				indicatorList->front().scale(.55, .55, .55);
			}
			else
			{
				printf("big fat miss lulz\n");
			}
		}
		index++;
		delete[] intersectResult;
	}
	return hitList;
}

GLfloat *Shape::getYAxis()
{
	GLfloat *ret = new GLfloat[3];
	
	ret[0] = this->curAxis.x;
	ret[1] = this->curAxis.y;
	ret[2] = this->curAxis.z;

	return ret;
}

void Shape::link(Shape *target) //links the objects together to ensure consistent motion & mechanics
{

}

void Shape::SET_MASTER()
{
	
}

/*
GLdouble *getAxes()
{

}
*/

const int Shape::getType()
{
	return type;
}

void deleteMatrix(GLdouble **M)
{
	for (int i = 0; i < 4; i++)
	{
		delete[] M[i];
	}
	delete[] M;
}

void Shape::setVS(int amt)
{
	this->vs = amt;
}

void Shape::setColor(GLfloat *clr)
{
	this->rgb[0] = clr[0];
	this->rgb[1] = clr[1];
	this->rgb[2] = clr[2];
}

Shape::~Shape() //destructor
{
	//printf("Calling shape destructor.\n");
}














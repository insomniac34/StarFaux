/********************************************************************************
* @Developer: Tyler Raborn
* @Course: CS1566 
* @Description: Method & Constructor definitions for my Camera class
* @Dev Environment: Microsoft Visual Studio 2012, Windows 8 Pro 64-bit
* @GPU: nVidia GeForce GTX 670 2GB
*********************************************************************************/

//DEFINITIONS:
#define _USE_MATH_DEFINES

//IMPORTS:
#include <cstdio>
#include <cmath>
#include <vector>
#include "Camera.h"
#include "Shape.h"
#define DEG 180.0/M_PI
#define RADS M_PI/180.0

//GLOBALS:
static const float Origin[3] = {0.0, 0.0, 0.0};
static float dH = 1.0;
extern float cam_x;
extern float cam_y;
extern float cam_z;

Camera::Camera()
{
	//it is Sacrilage to defile thine Defaultest of Constructors
}

Camera::Camera(GLdouble posx, GLdouble posy, GLdouble posz, GLdouble lookAtx, GLdouble lookAty, GLdouble lookAtz, GLdouble upx, GLdouble upy, GLdouble upz)
{
	rest=false;
	falling=false;
	dPerspNCP=0.0;
	dOrthoNCP=0.0;
	dPerspFCP=0.0;
	dOrthoFCP=0.0;
	dH=0.0;
	dAspect=0.0;
	zoomAmt=0.0;
	upy = 18.0;
	dy = 0.0;

	pX = posx;
	pY = posy;
	pZ = posz;

	this->lookPoint[0]=lookAtx;
	this->lookPoint[1]=lookAty;
	this->lookPoint[2]=lookAtz;

	GLdouble pos[3] = {posx, posy, posz};

	GLdouble up[3] = {upx, upy, upz};
	GLdouble look[3] = {lookAtx, lookAty, -lookAtz};
	GLdouble looklen = sqrt(pow(look[0], 2) + pow(look[1], 2) + pow(look[2], 2));

	for (int i = 0; i < 3; i++)
	{
		w[i] = ((-1 * look[i]) / looklen);
		if (w[i]==0) w[i]=0; //get rid of neg zeroes
	}

	GLdouble *vTemp = new GLdouble[3];

	for (int j = 0; j < 3; j++)
	{
		vTemp[j] = (up[j] - (dotProduct4d(up, w)*w[j]));	
	}

	GLdouble vLen = sqrt(pow(vTemp[0], 2) + pow(vTemp[1], 2) + pow(vTemp[2], 2));

	for (int k = 0; k < 3; k++)
	{
		v[k] = (vTemp[k] / vLen);
	}
	delete[] vTemp;

	GLdouble* uCross = crossProduct(v, w); //Since w and v are unit length and perpendicular, their cross product is also unit length.
	u[0] = uCross[0];
	u[1] = uCross[1];
	u[2] = uCross[2];

	delete[] uCross;// "And for each New array He declareth, He shall also have a matching Delete[], as a Shephard has a matching Flock" Stroustrup, 19:4
}

void Camera::transform() //accepts a pointer to a deque of Shapes. Multiplies each element of each shape's vertex data structure by the translation matrix T, thereby translating the camera to the world's origin...
{
	GLdouble Ttrans[16] = {
		                   1.0,       0.0,       0.0,       0.0, 
	                       0.0,       1.0,       0.0,       0.0,
	                       0.0,       0.0,       1.0,       0.0,
	                       (-1*pX),   (-1*pY),   (-1*pZ),   1.0,
	                      };
    
	glMultMatrixd(Ttrans); //update matrix stack           
}

void Camera::transformReturn()
{
	GLdouble Ttrans[16] = {
		                   1.0,       0.0,       0.0,       0.0, 
	                       0.0,       1.0,       0.0,       0.0,
	                       0.0,       0.0,       1.0,       0.0,
	                       (pX),      (pY),      (pZ),      1.0,
	                      };
    
	glMultMatrixd(Ttrans); //update matrix stack          
}


//then rotate the (u,v,w) vectors into (x, y, z)
void Camera::align()
{
	GLdouble Rrot[16] = {
		                 u[0], v[0], w[0], 0.0,
		                 u[1], v[1], w[1], 0.0,
		                 u[2], v[2], w[2], 0.0,
		                 0.0,  0.0,  0.0,  1.0,
		                }; 
    
	glMultMatrixd(Rrot); //update matrix stack
}

//Sortho
void Camera::scale(GLdouble farCP, GLdouble height, GLdouble width, GLdouble nearCP)
{
	//modify via user-changable parameters:
	height+=dH;
	nearCP+=dOrthoNCP;
	farCP+=dOrthoFCP;

	if (width<0 || height<0 || farCP<0) return;

	GLdouble Sortho[16] = {
		                   (2/width), 0.0,        0.0,       0.0,
		                   0.0,       (2/height), 0.0,       0.0,
		                   0.0,       0.0,        (1/farCP), 0.0,
		                   0.0,       0.0,        0.0,       1.0,
		                  };

    //GLdouble *S_ptr = &S;
	glMultMatrixd(Sortho); //push the scaling matrix S onto the stack in order to shrink the view wolume to its desired specs
}

//Sperspective
void Camera::scalePerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	//modify via user-changable parameters:
	fovy+=dH;
	zNear+=dPerspNCP;
	zFar+=dPerspFCP;
	aspect+=dAspect;

	this->far = zFar;

	if (zFar<0) return;

	GLdouble **Sperspective;
	GLdouble c = (-zNear / zFar);
	GLdouble THETAh = DEG*fovy;
	GLdouble THETAw = THETAh * aspect;

	if ((c+1)==0) return; 

	GLdouble Sxyz[16] = {
						 (1/(tan(THETAw/2)*zFar)),       0.0,       0.0,     0.0,
						 0.0,       (1/(tan(THETAh/2)*zFar)),       0.0,     0.0,
						 0.0,                            0.0,  (1/zFar),     0.0,
						 0.0,                            0.0,       0.0,     1.0,
						};

    GLdouble Mpp[16] = {
    				    1.0, 0.0, 0.0,         0.0,
    					0.0, 1.0, 0.0,         0.0,
    					0.0, 0.0, (-1/(c+1)), -1.0,
    					0.0, 0.0, (c/(c+1)),   0.0,
    				   };

    //Sperspective = matrixMult(Mpp, Sxyz);
    glMultMatrixd(Mpp);
    glMultMatrixd(Sxyz); //update matrix stack
}

void Camera::print() //print class information
{

}


void Camera::translateCamera(GLdouble x, GLdouble y, GLdouble z)
{
	this->pX+=x;
	this->pY+=y;
	this->pZ+=z;
	//this->transform();
}

void Camera::translateCameraLocalized(GLdouble x, GLdouble y, GLdouble z)
{
	float t = 1.0;

	GLdouble tempX = pX;
	GLdouble tempY = pY;
	GLdouble tempZ = pZ;

	if (y!=0)
	{
		this->pX+=(y*(t*v[0]));
		this->pY+=(y*(t*v[1]));
		this->pZ+=(y*(t*v[2]));		
	}
	else if (x!=0)
	{
		this->pX+=(x*(t*u[0]));
		this->pY+=(x*(t*u[1]));
		this->pZ+=(x*(t*u[2]));		
	}
	else
	{
		this->pX+=(z*(t*w[0]));
		this->pY+=(z*(t*w[1]));
		this->pZ+=(z*(t*w[2]));		
	}

	this->tX = (tempX-pX);
	this->tY = (tempY-pY);
	this->tZ = (tempZ-pZ);
}

void Camera::rotateCameraLocalized(GLdouble THETA, GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble **Rnew;
	THETA *= (M_PI/180);

	GLdouble Rrot[4][4] = {
	                       {u[0], u[1], u[2], 0.0},
	                       {v[0], v[1], v[2], 0.0},
	                       {w[0], w[1], w[2], 0.0},
	                       {0.0,  0.0,  0.0,  1.0},
	                      }; 

	GLdouble rotateX[4][4] = {
							  {1.0, 0.0,		0.0,	         0.0}, 
							  {0.0, cos(THETA), (-1*sin(THETA)), 0.0}, 
							  {0.0, sin(THETA), cos(THETA),      0.0}, 
							  {0.0, 0.0,	    0.0,	         1.0}, 
							 };

	GLdouble rotateY[4][4] = {
							  {cos(THETA),       0.0, sin(THETA), 0.0}, 
						      {0.0,              1.0, 0.0,        0.0}, 
						      {(-1*sin(THETA)),  0.0, cos(THETA), 0.0}, 
						      {0.0,              0.0, 0.0,        1.0}, 
					         };

	GLdouble rotateZ[4][4] = {
							  {cos(THETA), (-1*sin(THETA)), 0.0, 0.0}, 
						      {sin(THETA), cos(THETA),      0.0, 0.0}, 
						      {0.0,		   0.0,		        1.0, 0.0}, 
						      {0.0,		   0.0,		        0.0, 1.0}, 
					         };
    
	if (x==1 || x==-1)
	{
		Rnew = matrixMult(rotateX, Rrot);
	}
	else if (y==1 || y==-1)
	{
		Rnew = matrixMult(rotateY, Rrot);
	}
	else
	{
		Rnew = matrixMult(rotateZ, Rrot);
	}

	this->u[0] = Rnew[0][0];
	this->u[1] = Rnew[0][1];
	this->u[2] = Rnew[0][2];

	this->v[0] = Rnew[1][0];
	this->v[1] = Rnew[1][1];
	this->v[2] = Rnew[1][2];

	this->w[0] = Rnew[2][0];
	this->w[1] = Rnew[2][1];
	this->w[2] = Rnew[2][2];

	for (int i = 0; i < 4; i++)
	{
		delete[] Rnew[i];
	}

	delete[] Rnew;
}

void Camera::decrementHeight()
{
	dH--;
}

void Camera::incrementHeight()
{
	dH++;
}

void Camera::increaseNCP()
{
	this->dOrthoNCP++;
	this->dPerspNCP++;
}

void Camera::decreaseNCP()
{
	this->dOrthoNCP--;
	this->dPerspNCP--;
}

void Camera::increaseFCP()
{
	this->dOrthoFCP++;
	this->dPerspFCP++;
}

void Camera::decreaseFCP()
{
	this->dOrthoFCP--;
	this->dPerspFCP--;
}

void Camera::zoomIn()
{
	this->zoomAmt++;
}

void Camera::zoomOut()
{
	this->zoomAmt--;
}

void Camera::decreaseAR()
{
	this->dAspect--;
}

void Camera::increaseAR()
{
	this->dAspect++;
}

void Camera::jump(GLdouble h)
{
	if (pY<.5) dy*=-1.0; //reverse velocity when ground is hit.
	dy+=0.5; //apply resistance.
	pY-=dy; //update y pos
	if (pY < 0.0) pY=0.0; //correct any negative values resulting from incrementing past zero.
	//printf("y = %g dy = %g\n", pY, dy);
	if ((pY==0) && (abs(dy)<=4)) rest=true; //if the object settles on Y=0 w/ an increment less than 10, halt further updates. The ball/rectangle is at rest.
}

void Camera::crouch(GLdouble h)
{
	pY-=h;
	if (pY < -2) pY = -2;
}

//Shape friend methods:
void Camera::synchronizedRotate(const GLdouble deg, Shape *target)
{	
	target->arbitraryRotate(deg, this->v[0], this->v[1], this->v[2], this->pX, this->pY, this->pZ);
	this->rotateCameraLocalized((-1*deg), 0.0, 1.0, 0.0);
}

void Camera::synchronizedRotate(const GLdouble deg, Shape3D *target)
{	
	target->local_rotate_y((-1*deg)*RADS);
	this->rotateCameraLocalized(deg*.75, 0.0, 1.0, 0.0);

	//this->rotateCameraLocalized(.1, 0.0, 0.0, 1.0);	
}

void Camera::synchronizedRotate(const GLdouble deg, std::deque<Shape*> targetList) //accepts a deque loaded with pointers to shapes to rotate/translate the camera in relation to.
{
	for (std::deque<Shape*>::iterator iter = targetList.begin(); iter < targetList.end(); iter++)
	{
		(*iter)->arbitraryRotate(deg, this->v[0], this->v[1], this->v[2], this->pX, this->pY, this->pZ);
	}

	this->rotateCameraLocalized((-1*deg), 0.0, 1.0, 0.0);
}

void Camera::synchronizedTranslate(const GLdouble x, const GLdouble y, const GLdouble z, Shape *target) 
{

	Vertex shapeTempX = Vertex(target->curXAxis.x, target->curXAxis.y, target->curXAxis.z);
	Vertex shapeTempY = Vertex(target->curAxis.x, target->curAxis.y, target->curAxis.z);
	Vertex shapeTempZ = Vertex(target->curZAxis.x, target->curZAxis.y, target->curZAxis.z);

	target->curXAxis = Vertex(this->u[0], this->u[1], this->u[2]);
	target->curAxis = Vertex(this->v[0], this->v[1], this->v[2]);
	target->curZAxis = Vertex(this->w[0], this->w[1], this->w[2]);

	target->translate(x, y, z);
	this->translateCamera((1*x), (1*y), (1*z));

	target->curXAxis = Vertex(shapeTempX.x, shapeTempX.y, shapeTempX.z);
	target->curAxis = Vertex(shapeTempY.x, shapeTempY.y, shapeTempY.z);
	target->curZAxis = Vertex(shapeTempZ.x, shapeTempZ.y, shapeTempZ.z);
}

void Camera::synchronizedTranslate(const GLdouble x, const GLdouble y, const GLdouble z, Shape3D *target)
{
	target->world_translate(x, y, z);
	this->translateCamera(x, y, z);
	cam_x += x;
	cam_y += y;
	cam_z += z;
}

void Camera::synchronizedTranslate(const GLdouble x, const GLdouble y, const GLdouble z, std::deque<Shape*> targetList) //accepts a deque loaded with pointers to shapes to rotate/translate the camera in relation to.
{
	for (std::deque<Shape*>::iterator iter = targetList.begin(); iter < targetList.end(); iter++)
	{
		(*iter)->translate(x, y, z);
	}

	this->translateCameraLocalized((-1*x), (-1*y), (-1*z));
}

GLdouble Camera::getFar()
{
	return this->far;
}

GLdouble* Camera::getPosition()
{
	GLdouble *ret = new GLdouble[3];
	ret[0] = this->pX;
	ret[1] = this->pY;
	ret[2] = this->pZ;

	return ret;
}

GLdouble* Camera::getLookPoint()
{
	GLdouble *ret = new GLdouble[3];
	ret[0] = (this->pX - (.06 * w[0]));
	ret[1] = (this->pY - (.06 * w[1]));
	ret[2] = (this->pZ - (.06 * w[2]));

	return ret;
}

void Camera::paintLook()
{

}

Camera::~Camera() 
{

}
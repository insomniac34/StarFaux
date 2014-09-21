#include "Light.h"

Light::Light()
{

}

Light::Light(GLdouble args[13])
{
	this->amb[0] = args[0];
	this->amb[1] = args[1];
	this->amb[2] = args[2];
	
	this->diff[0] = args[3];
	this->diff[1] = args[4];
	this->diff[2] = args[5];

	this->specular[0] = args[6];
	this->specular[1] = args[7];
	this->specular[2] = args[8];
	
	this->position[0] = args[9];
	this->position[1] = args[10];
	this->position[2] = args[11];
}

GLdouble Light::getShine()
{
	return this->shine;
}

GLdouble *Light::getAmbient()
{
	GLdouble *ret = new GLdouble[3];
	ret[0] = this->amb[0];
	ret[1] = this->amb[1];
	ret[2] = this->amb[2];

	return ret;
}

Light::~Light()
{

}
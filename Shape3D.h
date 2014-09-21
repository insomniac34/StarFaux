/****************************************************
 FILE:		Shape3D.h
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		October 26, 2013
 DESC:		Header file for Shape3D.cpp
 ****************************************************/

#ifndef _SHAPE3D_H_
#define _SHAPE3D_H_

#define WIREFRAME 0
#define SOLID 1
#define NORMALS 4
#define LOCAL_AXES 8

#include <iostream>
#include <cmath>
#include <deque>
#include <cstdio>
#include <cstring>
#include "matrixmath.h"
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

class Shape3D
{
public:
	// local coordinate system
	GLfloat local_x[4];
	GLfloat local_y[4];
	GLfloat local_z[4];
	GLfloat local_p[4];
	
	// scale info
	GLfloat scale[4];
	GLfloat rotation[4];
	
	// vertices
	GLfloat** verts;
	int verts_size;
	
	// faces
	int** faces;
	int faces_size;
	
	// vertex normals
	GLfloat** v_norms;
	
	// face normals
	GLfloat** f_norms;
	
	// color (with alpha)
	float color[4];
	
	int spawnLock;
	
	float** face_colors;
	
	typedef struct flash_face
	{
		int face_num;
		float rstart, gstart, bstart;
		float rend, gend, bend;
		float r, g, b;
	} flash_face;
	
	deque<flash_face*> flashing;
	
	// lighting info
	GLfloat shine;
  GLfloat emi[4];
  GLfloat amb[4];
  GLfloat diff[4];
  GLfloat spec[4];
	
	// constructor, destructor
	Shape3D();
	~Shape3D() {};
	
	GLfloat* getPosition();
	
	// set/get color
	void set_color4f(float r, float g, float b, float a);
	void set_color(float c[4]);
	float* get_color();
	
	// draw
	void draw(int draw_mode);
	
	// applies a matrix to all vertices
	void apply_verts(float m[16]);
	
	// applies a matrix to face and vertex normals
	void apply_norms(float m[16]);
	
	// vertex transformations
	void reset_to_origin();
	void v_translate(GLfloat v[4], float x, float y, float z);
	void v_rotate_x(GLfloat v[4], float degrees);
	void v_rotate_y(GLfloat v[4], float degrees);
	void v_rotate_z(GLfloat v[4], float degrees);
	
	// local transformations
	void local_scale(float x, float y, float z);
	void local_rotate_x(float degrees);
	void local_rotate_y(float degrees);
	void local_rotate_z(float degrees);
	void arbitrary_rotate(float degrees, float* axis_p, float* axis_v);
	void local_translate(float x, float y, float z);
	
	// world transformations
	void world_scale(float x, float y, float z);
	void world_rotate_x(float theta);
	void world_rotate_y(float theta);
	void world_rotate_z(float theta);
	void world_translate(float x, float y, float z);
	
	void calc_face_normal(int face_index);
	void set_face(int f_pos, int pos, int pos2, int pos3, int vert_faces[]);
	
	virtual void load_mesh(char* the_mesh) = 0;
	virtual void draw_mesh(int draw_mode, int shade_mode) = 0;
	virtual void set_flashing(int face_num, float r, float g, float b, float speed) = 0;
	virtual void update() = 0;
};


// Derived Classes

class Cube: public Shape3D
{
private:
	// size of the cube
	float h;
	
public:
	// constructor, destructor
	Cube(float my_size);
	~Cube() {};
	// applies vertex coordinates and initalizes other vertex values
	void make_vert(int vert_num, float x, float y, float z);
	// creates face and calculates the normal
	void make_face(int face_num, int v1, int v2, int v3);
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class Sphere: public Shape3D
{
private:
	float radius;
	
public:
	// constructor, destructor
	Sphere(float r, int vs, int rs);
	~Sphere() {};
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class Cylinder: public Shape3D
{
private:
	float radius;
	float height;
	
public:
	// constructor, destructor
	Cylinder(float r, float h, int vs, int rs);
	~Cylinder() {};
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class Cone: public Shape3D
{
private:
	float radius;
	float height;
	
public:
	// constructor, destructor
	Cone(float r, float h, int vs, int rs);
	~Cone() {};
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class Torus: public Shape3D
{
private:
	// radius of the ring
	float radius;
	// thickness of the ring
	float radius2;
	
public:
	// constructor, destructor
	Torus(float r, float r2, int vs, int rs);
	~Torus() {};
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class House: public Shape3D
{
private:
	// size of the house
	float h;
	
public:
	// constructor, destructor
	House(float my_size);
	~House() {};
	// applies vertex coordinates and initalizes other vertex values
	void make_vert(int vert_num, float x, float y, float z);
	// creates face and calculates the normal
	void make_face(int face_num, int v1, int v2, int v3);
	void load_mesh(char* the_mesh) {};
	void draw_mesh(int draw_mode, int shade_mode) {};
	void set_flashing(int face_num, float r, float g, float b, float speed) {};
	void update() {};
};

class Mesh: public Shape3D
{
public:
	Mesh() {};
	~Mesh() {};
	
	virtual void load_mesh(char* the_mesh);
	virtual void set_flashing(int face_num, float r, float g, float b, float speed);
	void parse_count(char buffer[], int* vert_faces);
	void parse_vert(char buffer[], int i);
	void parse_face(char buffer[], int i, int* vert_faces);
	virtual void draw_mesh(int draw_mode, int shade_mode);
	virtual void update();
};


#endif
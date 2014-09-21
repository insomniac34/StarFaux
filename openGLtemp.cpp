/****************************************************
FILE:		openGLtemp.cpp
PLATFORM:	Mac OS X
AUTHOR:		Charles Koch
DATE:		September 22, 2013
DESC:		Template file for OpenGL projects.
****************************************************/

/*
Xcode Instructions:
	* create new project from Empty Project
	* right-click Targets, select Add->New Target
	* select Cocoa Application
	* in User Defined settings, delete GCC_PREFIX_HEADER
	* right-click the new target, select Add->Existing Frameworks
	* command-click OpenGL and GLUT Frameworks
*/

#include <cstdlib>
#include <iostream>
#include <cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "Shape3D.h"
#include "player.h"
#include "SOIL/soil.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
using namespace std;

GLuint tex1;

Shape3D* my_shape;

int health_bar2 = 191;

player* player1;

float cam_x2 = 0;
float cam_y2 = 0;
float cam_z2 = 15;
float look_x2 = 0;
float look_y2 = 0;
float look_z2 = 0;

int my_shading = GL_FLAT;

void my_display()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1.0, 1.0, 100.0);
	look_x2 = player1->ship->local_p[0]/4.0;
	look_y2 = player1->ship->local_p[1]/4.0;
	look_z2 = player1->ship->local_p[2];
	gluLookAt(cam_x2, cam_y2, cam_z2, look_x2, look_y2, look_z2, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
	
	GLfloat my_amb[4] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat my_diff[4] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat my_spec[4] = { 0.2, 0.2, 0.2, 1.0 };
	if(my_shading == GL_FLAT)
	{
		my_diff[0] = 1;
		my_diff[1] = 1;
		my_diff[2] = 1;
		my_spec[0] = 0;
		my_spec[1] = 0;
		my_spec[2] = 0;
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, my_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, my_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, my_spec);
	player1->ship->draw_mesh(GL_TRIANGLES, my_shading);
	my_shape->draw_mesh(GL_TRIANGLES, my_shading);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	
	glColor3f(1, 0, 0);
	int health_bar_len = health_bar2 * player1->health + 57;
	glRecti(57, 56, health_bar_len, 80);
	
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, tex1);
	
	
	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(296, 296, 0);
	glTexCoord2f(0, 0); glVertex3f(40, 296, 0);
	glTexCoord2f(0, 1); glVertex3f(40, 40, 0);
	glTexCoord2f(1, 1); glVertex3f(296, 40, 0);
	glEnd();
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glutSwapBuffers();
	return;
}

void my_reshape(int w, int h)
{
	glViewport(0, 0, min(w, h), min(w, h));
	return;
}

void my_mouse(int button, int state, int mousex, int mousey)
{
	return;
}

void my_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':
			player1->ship->local_rotate_x(-M_PI/25);
			break;
		case 's':
			player1->ship->local_rotate_x(M_PI/25);
			break;
		case 'q':
			player1->ship->local_rotate_y(M_PI/25);
			break;
		case 'e':
			player1->ship->local_rotate_y(-M_PI/25);
			break;
		case 'a':
			player1->ship->local_rotate_z(-M_PI/25);
			break;
		case 'd':
			player1->ship->local_rotate_z(M_PI/25);
			break;
		case ' ':
			if(my_shading == GL_FLAT)
			{
				glShadeModel(GL_SMOOTH);
				my_shading = GL_SMOOTH;
			}
			else
			{
				glShadeModel(GL_FLAT);
				my_shading = GL_FLAT;
			}
			break;
	}
	return;
}


void keyDown(unsigned char key, int x, int y)
{
	player1->keyPressed(key);
}

void keyUp(unsigned char key, int x, int y)
{
	player1->keyReleased(key);
}

void my_timer(int id)
{
	float o[3] = { 0, 0, 0 };
	player1->update(o);
	glutPostRedisplay();
	glutTimerFunc(20, my_timer, 0);
	return;
}

void glut_setup2()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("title");

	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutMouseFunc(my_mouse);
	//glutKeyboardFunc(my_keyboard);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutTimerFunc(20, my_timer, 0);
	return;
}

void gl_setup2()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1.0, 1.0, 100.0);
	gluLookAt(0, 0, 15, 0, 0, 0, 0, 1, 0);
	
	GLfloat globalAmb[]     = {.1, .1, .1, .1};
	
	GLfloat amb[4]={ 0.1, 0.1, 0.1, 1 };
	GLfloat diff[4]={ 1, 1, 1, 1 };
	GLfloat spec[4]={ 1, 1, 1, 1 };
	GLfloat light_pos[4]={ 0, 35, 0, 1 };
	GLfloat light_dir[4]={ 0, 1, 0, 1};

  //enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
	
  // reflective propoerites -- global ambiant light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	
	glShadeModel(GL_FLAT);
	return;
}

void my_setup()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	int width = 256; 
	int height = 256;
	unsigned char* image = SOIL_load_image("shield_bar.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0, 0.0, 0.0, 0.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	
	player1 = new player();
	my_shape = new Mesh();
	char mesh_file[50];
	strcpy(mesh_file, "cube_mesh.txt");
	my_shape->load_mesh(mesh_file);
	my_shape->world_translate(0, 0, -15);
	return;
}

/*int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glut_setup2();
	gl_setup2();
	my_setup();

	glutMainLoop();

	return(0);
}
*/
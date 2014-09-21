/*
		Written by Tyler Raborn & Charlie Koch

				this is the entrypoint into our game. As in, MAIN.

	This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.
*/

//C++ headers:
#include <deque> //Tyler: literally the best data structure ever...like an ArrayList but without that whole Java mess! ;)
#include <string>
#include <list>
#include <memory>
#include <algorithm>

//C headers:
#define _USE_MATH_DEFINES	
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "stdio.h"

//MUST make sure that the Apple Magical Fairy Princess is sufficiently pleased -tyler
#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//local headers:
#include "Light.h"
#include "Shape.h"
#include "Projectile.h"
#include "vec3.h"
#include "Vertex.h"
#include "Camera.h"
#include "NPC.h"
#include "Path.h"
#include "Shape3D.h"
#include "player.h"
#include "Billboard.h"
#include "Menu.h"
#include "SOIL/SOIL.h"
#include "PowerUp.h"
//#include "Model.h"

#define SHAPE_CUBE 0
#define SHAPE_CYLINDER 1
#define SHAPE_SPHERE 2
#define SHAPE_TORUS 3
#define SHAPE_CONE 4

#define NPC_HOSTILE 0
#define NPC_NEUTRAL 1
#define NPC_FRIENDLY 2

#ifndef _SPAWN_
#define _SPAWN_

typedef struct _Spawn //idea is to create the full-fledged NPC, along with its Path and Shape objects, load them into the Zone struct, and they eventually will be loaded into the current world at the appropriate time (e.g. when that zone is reached)
{
	Shape3D *target; //handle to actual instantiated NPC, to be added to the scenegraph when that zone is loaded
	Vertex offset; //offset from camera position to draw the object at
} Spawn;

#endif

#ifndef _ZONE_
#define _ZONE_

typedef struct _Zone //each "sublevel" can be described via a Zone; it contains a list of static objects (buildings/trees etc...) and a list of enemies and when/where they spawn
{
	std::deque<Shape3D*> objectList;
	std::deque<Spawn> NPCList;
	std::deque<PowerUp*> powerUpList;
	
} Zone;	

#endif	

//OpenGL/GLUT Application Entrypoints
int main(int, char**);
void glut_setup();
void gl_setup(void);

//openGL/GLUT callback funcs
void gl_display(void);
void gl_reshape(int width, int height);
void gl_mouse(int button, int state, int mouseX, int mouseY);
void gl_mouse_drag(int mouseX, int mouseY);
void gl_keyboard(unsigned char key, int x, int y);
void gl_keyboard_up(unsigned char key, int x, int y);
void gl_special_keys(int key, int x, int y);
void gl_timer(int timerID);
void gl_idle(void);
void gl_cleanup(void);

void moveCamera(GLfloat, GLfloat, GLfloat);
void turnCamera(GLfloat, int, int, int);
void realTimeCleanup();
void playerFire();
void createStaticObjects();
void drawStaticObjects();
void animateProjectiles();
void animateThrusters();
void worldRedraw();

void updatePowerUps();

void spawnEnemy(GLdouble*);
void NPCupdate();

void arwing_setup();
void updateFriendlies();
void checkTriggers();
void drawBillboards();

/*
        a non-player character 
                                
                    Written by Tyler Raborn

*/

//C++ headers
#include <deque>
#include <list>

//C headers

//OpenGL Headers
#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

//local headers             
#include "Shape.h"
#include "Shape3D.h"
#include "Path.h"
#include "PowerUp.h"

#include "vec3.h"

#define NPC_HOSTILE 0
#define NPC_NEUTRAL 1
#define NPC_FRIENDLY 2  

#define LASER_SPEED_SUPER_SLOW 2.0
#define LASER_SPEED_SLOW 10.0
#define LASER_SPEED_MEDIUM 20.0
#define LASER_SPEED_FAST 30.0               

//objDef
#ifndef _NPC_H
#define _NPC_H

class Projectile;

class NPC
{

public:
    
    NPC();
    //explicit NPC(int, Shape*, Path*);
    explicit NPC(int, int, Shape3D*, Path*);
    ~NPC();

    Shape3D *getShape(); //returns a pointer to the Shape object for this NPC
    Path *getPath(); //returns a pointer to the Path object allocated for this NPC
    GLfloat *getPosition();
    void update(GLfloat, std::deque<Projectile>*, GLfloat*, int*, std::deque<PowerUp*>*, std::list<NPC*>*, std::list<NPC*>*, bool); 
    void adjustAltitude(float);
    void adjustBearing(float);
    bool pathComplete;
    int getIFF();
    float health;
    int spawnLock;
    int isDead;

    int colorFlag;
    GLfloat explosionWidthX;
    GLfloat explosionWidthY;
    GLfloat explosionWidthZ;
    
    Shape3D *targetShape;

    int AI;

private:

    //Shape3D *targetShape3D;
    Path *targetPath;
    vec3 currentDirection;
    GLfloat currentWaypoint[3];
    std::deque<Vertex>::iterator waypointIterator;
    int iff;
    int fireLock;
    int fireCounter;

};

#endif

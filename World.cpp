/********************************************************************************************
    Charlie Koch & Tyler Raborn Present:

        ,-.      .-,
        |-.\ __ /.-|
        \  `    `  /
        / _     _  \
        | _`q  p _ |
        '._=/  \=_.'
          {`\()/`}`\
          {      }  \
          |{    }    \
          \ '--'   .- \
          |-      /    \
          | | | | |     ;
          | | |.;.,..__ |
        .-"";`         `|
       /    |           /
       `-../____,..---'`

           STAR FAUX ENHANCED EDITION

    CS1566 Intro to Graphics Final Project

        This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.

********************************************************************************************/

#include "World.h"
#include "config.h"         

//static std::string WINDOW_TITLE = "STAR FAUX ENHANCED EDITION (OpenGL)";

static Light *light0;
static Light *light1;
static Light *light2;

static bool ACTIVATE_CUTSCENE = false;

static GLfloat GREEN[3] = {0.15, 0.5, 0.1};
static GLfloat DARK_GREEN[3] = {0.0, 0.3, 0.0};
static GLfloat PURPLE[3] = {0.6, 0.1, 0.6};
static GLfloat RED[3] = {1.0, 0.0, 0.0};
static GLfloat GREY[3] = {.45, .43, .43};
static GLfloat DARK_GREY[3] = {.33, .33, .33};
static GLfloat YELLOW[3] = {1.0, 1.0, 0.0};
static GLfloat BLUE[3] = {0.0, 0.0, 0.9};
static GLfloat LIGHT_BLUE[3] = {0, 0.6, 1.0};
static GLfloat BROWN[3] = {.35, .30, .25};
static GLfloat WHITE[3] = {1.0, 1.0, 1.0};

static GLfloat DARK_GREY_3D[4] = {.5, .5, .5, 1.0};
static GLfloat GREEN_3D[4] = {0.15, 0.5, 0.1};
static GLfloat DARK_GREEN_3D[4] = {0.0, 0.6, 0.0, 1.0};
static GLfloat PURPLE_3D[4] = {0.6, 0.1, 0.6, 1.0};
static GLfloat RED_3D[4] = {1.0, 0.0, 0.0, 1.0};
static GLfloat GREY_3D[4] = {.45, .43, .43, 1.0};
static GLfloat YELLOW_3D[4] = {1.0, 1.0, 0.0, 1.0};
static GLfloat BLUE_3D[4] = {0.0, 0.0, 0.9, 1.0};
static GLfloat LIGHT_BLUE_3D[4] = {0.0, 0.4, 1.0, 1.0};
static GLfloat BROWN_3D[4] = {.35, .30, .25, 1.0};
static GLfloat WHITE_3D[4] = {1.0, 1.0, 1.0, 1.0};

static char *arwingFile = "arwing.txt";
static GLfloat playerSpeed = -0.4;
static GLfloat playerAcceleration = -1.0;
static GLfloat playerDeceleration = -0.1; 
static GLfloat friendlySpeed = -0.4;
static GLfloat hostileSpeed = 0.4;

static GLfloat playerProximityBoost = 0.0;
int boostFlag = 0;

bool PAUSED = false;
bool QUIT_GAME = false;
int playerHit;

//Global Data Structures & Iterators:
static std::deque<Projectile> projectileList;
static std::deque<Shape*> dynamicSceneGraph;
static std::deque<Billboard*> billboardList;
static std::list<Shape3D*> staticSceneGraph; //global data structure of shapes NOT meant to be destroyed. Eventually im going to set it up to iterate over this list and destroy objects not in view.
static std::list<NPC*> enemyList;
static std::list<NPC*> friendList;
static std::deque<Zone> areaList;
static std::deque<Zone>::iterator zoneIterator;

//POINTERS. Pointers, everywhere!
static player* player1;
static Camera *playerCam;

static NPC *frog;
static NPC *rabbit;
static NPC *falco;
static NPC *friendly;

float cam_x = 0;
float cam_y = 0;
float cam_z = 15;
float look_x = 0;
float look_y = 0;
float look_z = 0;
int enemies_killed;

////////////////////////////////Flags////////////////////////////////////////////////////
static int objectFlag;
static int drawFlag;
static int fireFlag;
static int thrustFlag;
static int deletionLock;
static int spawnLock;
static int fireCounter;
static int zoneCounter;
int game_status = GAME_START;
static int pause_timer = 0;
int my_shade;

//control flags for smooth response 
int INCREASE_SPEED;
int DECREASE_SPEED;
static int TURN_LEFT;
static int TURN_RIGHT;
int FIRE_LASERS;
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////Texture Flags///////////////////////////////////////////
GLuint hp, bp;
int health_bar = 191;
////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    //initialize globals
    objectFlag = 1;
    drawFlag = 0;
    fireFlag = 0;
    thrustFlag = 0;
    deletionLock = 0;
    spawnLock = 0;
    fireCounter = 0;
    zoneCounter = 0;

    //smoothness flags:
    INCREASE_SPEED = 0;
    DECREASE_SPEED = 0;
    TURN_LEFT = 0;
    TURN_RIGHT = 0;
    FIRE_LASERS = 0;

    glutInit(&argc, argv);
    glut_setup();
    gl_setup();

    //////////////////////////////////MAIN GAME LOOP////////////////////////////////
    glutMainLoop();
    ////////////////////////////////////////////////////////////////////////////////

    return 0;
}

void loadTextures()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &hp);
    glBindTexture(GL_TEXTURE_2D, hp);
    int width = 256; 
    int height = 256;

    unsigned char* image = SOIL_load_image(
                                           "shield_bar.png",
                                           &width,
                                           &height,
                                           0,
                                           SOIL_LOAD_RGBA
                                          );

    glTexImage2D(
                 GL_TEXTURE_2D,
                 0, 
                 GL_RGBA,
                 width, height,
                 0,
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE, 
                 image
                );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float color[] = { 0.0, 0.0, 0.0, 0.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SOIL_free_image_data(image);
    
    glGenTextures(1, &bp);
    glBindTexture(GL_TEXTURE_2D, bp);
    width = 256; 
    height = 256;

    image = SOIL_load_image(
                            "boost_bar.png",
                            &width, &height,
                            0, 
                            SOIL_LOAD_RGBA
                           );
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA, 
                 width, 
                 height, 
                 0, 
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE, 
                 image
                );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SOIL_free_image_data(image);
}

void drawTextures()
{
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(
            0.0,
            glutGet(GLUT_WINDOW_WIDTH),
            0.0, 
            glutGet(GLUT_WINDOW_HEIGHT), 
            -1.0, 
            1.0
           );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    if(playerHit == 1)
    {
        printf("PLAYER HIT DETECTEd!\n");
        glColor4f(1, 0, 0, 0);
        glRecti(0, 0, 1200, 800);
        playerHit = 0;
    }
    
    glColor4f(1, 0, 0, 1);
    int health_bar_len = health_bar * player1->health + 57;
    glRecti(57, 56, health_bar_len, 80);
    
    glColor4f(0.8, 0, 0.8, 1);
    health_bar_len = health_bar * player1->boost;

    glRecti(
            glutGet(GLUT_WINDOW_WIDTH) - 245, 
            56, 
            glutGet(GLUT_WINDOW_WIDTH) - 245 + health_bar_len, 
            80
           );

    glColor3f(0,0,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, hp);
    
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0); glVertex3f(296, 296, 0);
    glTexCoord2f(0, 0); glVertex3f(40, 296, 0);
    glTexCoord2f(0, 1); glVertex3f(40, 40, 0);
    glTexCoord2f(1, 1); glVertex3f(296, 40, 0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, bp);

    glBegin(GL_QUADS);
    glTexCoord2f(1, 0); glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 40, 296, 0);
    glTexCoord2f(0, 0); glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 296, 296, 0);
    glTexCoord2f(0, 1); glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 296, 40, 0);
    glTexCoord2f(1, 1); glVertex3f(glutGet(GLUT_WINDOW_WIDTH) - 40, 40, 0);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void my_keyDown(unsigned char key, int x, int y)
{
    if(key == 'q' )
    {
        atexit(gl_cleanup);
        exit(0);
    }
    if(key == 'p' && game_status == GAME_ON)
    {
        if (pause_timer == 0 && !PAUSED)
        {
            PAUSED = true;
            pause_timer = 1;
        }
    }

    player1->keyPressed(key);
}

void my_keyUp(unsigned char key, int x, int y)
{
    player1->keyReleased(key);

    if(key == 'p' && game_status == GAME_ON)
    {
        pause_timer = 0;
    }
}

void glut_setup()
{
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
    glutCreateWindow("StarFaux (OpenGL)");

    //party like it's 1999
    glutDisplayFunc(gl_display);
    glutTimerFunc(200, gl_timer, 0);
    glutReshapeFunc(gl_reshape);
    glutMouseFunc(gl_mouse);
    glutMotionFunc(gl_mouse_drag);
    glutKeyboardFunc(my_keyDown);
    glutKeyboardUpFunc(my_keyUp);
}

void gl_setup(void)
{
    init_menus();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    //BACKGROUND COLOR:
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    playerCam = new Camera(
                           0.0, 0.0, 200.0,
                           0.0, 0.0, 189.0, 
                           0.0, 1.0, 0.0
                          );

    cam_x = 0.0;
    cam_y = 0.0;
    cam_z = 200.0;
    look_x = 0.0;
    look_y = 0.0;
    look_z = -189.0;
    my_shade = GL_FLAT;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat globalAmb[]     = {.1, .1, .1, .1};
    
    GLfloat amb[4]={ 0.1, 0.1, 0.1, 1 };
    GLfloat diff[4]={ 1, 1, 1, 1 };
    GLfloat spec[4]={ 0, 0, 0, 1 };
    GLfloat light_pos[4]={ 0, 300, 100, 1 };
    GLfloat light_dir[4]={ 0, 1, 0, 1};
    float m_ang = 180.0;
    
    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, m_ang);

    glShadeModel(my_shade);

    loadTextures();
    createStaticObjects(); //load static objects into scene graph

    GLdouble *camPos = playerCam->getPosition();

    player1 = new player();
    player1->ship->world_translate(camPos[0], camPos[1]-1, camPos[2]-6);

    Shape3D *frogArwing;
    char mesh_file[50];
    strcpy(mesh_file, arwingFile);
    frogArwing = new Mesh();
    frogArwing->load_mesh(mesh_file);

    frogArwing->local_translate(
                                (camPos[0] - 5.0), 
                                (camPos[1] - 1), 
                                (camPos[2] - 9)
                               );

    frogArwing->local_rotate_y(M_PI);
    //frogArwing->local_rotate();

    Path *frogPath = new Path(
                              Vertex(camPos[0] - 5.0, camPos[1] - 1.0, camPos[2] - 9), 
                              Vertex(camPos[0] - 5.0, camPos[1] - 1.0, (camPos[2] - 9) - 250.0)
                             );

    frog = new NPC(
                   NPC_FRIENDLY, 
                   AI_DUMB, 
                   frogArwing, 
                   frogPath
                  );

    friendList.push_front(frog);



    Shape3D *friendlyArwing;
    //char mesh_file[50];
    strcpy(mesh_file, arwingFile);
    friendlyArwing = new Mesh();
    friendlyArwing->load_mesh(mesh_file);

    friendlyArwing->local_translate(    
                                    (camPos[0] + 5.0), 
                                    (camPos[1] + 1), 
                                    (camPos[2] - 9)                                 
                                   );

    friendlyArwing->local_rotate_y(M_PI);
    //frogArwing->local_rotate();

    Path *friendlyPath = new Path(
                                  Vertex(camPos[0] + 5.0, camPos[1] + 1.0, camPos[2] - 9), 
                                  Vertex(camPos[0] + 5.0, camPos[1] + 1.0, (camPos[2] - 9) - 248.0)
                                 );

    friendly = new NPC(NPC_FRIENDLY, AI_DUMB, friendlyArwing, friendlyPath);
    //friendList.push_front(friendly);
    
    delete[] camPos;
}

void gl_display(void)
{
    GLdouble* cameraPosition;
    cameraPosition = playerCam->getPosition(); //keep track of the camera position for events etc...

    if(game_status == GAME_START)
    {
        game_start();
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(
                       GL_FOV, 
                       GL_ASPECT_RATIO, 
                       GL_NEAR_CLIPPING_PLANE, 
                       GL_FAR_CLIPPING_PLANE
                      );

        look_x = (player1->ship->local_p[0] / 4.0);
        look_y = player1->ship->local_p[1] / 4.0;
        look_z = player1->ship->local_p[2];

        gluLookAt(
                  cam_x,
                  cam_y, 
                  cam_z, 
                  look_x, 
                  look_y, 
                  look_z, 
                  0, 
                  1, 
                  0
                 );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
        
        GLfloat my_amb[4] = { 0.4, 0.4, 0.4, 1.0 };
        GLfloat my_diff[4] = { 1, 1, 1, 1.0 };
        GLfloat my_spec[4] = { 0.0, 0.0, 0.0, 1.0 };
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, my_amb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, my_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, my_spec);
        
        float cam_pos[3] = {cameraPosition[0], cameraPosition[1], cameraPosition[2]};

        if(PAUSED == false)
        {
            player1->update(cam_pos);
        }

        if (cameraPosition[2] < -50.0)
        {
            worldRedraw();
        }

        drawStaticObjects();

        if(PAUSED == false)
        {
            checkTriggers();
        }

        realTimeCleanup();

        if(PAUSED == false)
        {
            if (spawnLock == 0 && cameraPosition[2] < 100) //initial pair of enemy spawns
            {
                //printf("SPAWNING ENEMY!!!\n");
                spawnEnemy(cameraPosition);     
                spawnLock = 1;
            }
        }

        NPCupdate();
        glDisable(GL_LIGHTING);
        animateProjectiles();
        if (!PAUSED) updatePowerUps();
        glEnable(GL_LIGHTING);

        if (!PAUSED)
        {
            updateFriendlies();
        }

        if(game_status != GAME_OVER)
        {
            player1->ship->draw_mesh(GL_TRIANGLES, my_shade);
        }

        if(PAUSED == false) //advance player forward
        {
            playerCam->synchronizedTranslate(
                                             0,
                                             0,
                                             playerSpeed, 
                                             player1->ship
                                            );
            if (ACTIVATE_CUTSCENE == true)
            {
                for(std::list<NPC*>::iterator iter = friendList.begin(); iter != friendList.end(); iter++)
                {
                    playerCam->synchronizedTranslate(
                                                     0,
                                                     0,
                                                     playerSpeed, 
                                                     (*iter)->getShape()
                                                    );                  
                }
            }
        }

        drawTextures();
    }
    
    if(PAUSED == true && game_status == GAME_ON)
    {
        pause_screen();
    }

    if(game_status == GAME_WON)
    {
        PAUSED = true;
        int my_health = player1->health * 100;
        game_won(enemies_killed, my_health);
    }

    if(game_status == GAME_OVER)
    {
        PAUSED = true;
        game_over(player1->ship->getPosition());
    }
    
    glutSwapBuffers();
    delete[] cameraPosition;
}

void gl_reshape(int width, int height)
{

}

void gl_mouse(int button, int state, int x, int y)
{

}

void gl_mouse_drag(int mouseX, int mouseY)
{

}

void gl_timer(int timerID)
{
    if(PAUSED == false)
    {
        if (FIRE_LASERS==1)
        {
            playerFire();
        }

        if (INCREASE_SPEED==1)
        {
           playerSpeed = playerAcceleration;
           animateThrusters();      
        }
        else if (DECREASE_SPEED==1)
        {
            playerSpeed = playerDeceleration;
        }
        else
        {
            dynamicSceneGraph.clear();
            playerSpeed = DEFAULT_SPEED;
        }

        if(QUIT_GAME == true)
        {
            atexit(gl_cleanup);
            exit(0);
        }

        if(player1->health < 0)
        {
            player1->health = 0;
            game_status = GAME_OVER;
        }

        if (TURN_RIGHT==1)
        {
            turnCamera(TURN_RATE, 0, 1, 0);
        }

        if (TURN_LEFT==1)
        {
            turnCamera(-TURN_RATE, 0, 1, 0);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, gl_timer, 0);
}

void gl_idle(void)
{

}

void gl_cleanup(void) //ALL HEAP DEALLOCATIONS SHOULD GO IN HERE -tyler
{
    //printf("\n***initiating destructor subroutine***\n\n");

    for (std::deque<Projectile>::iterator iter = projectileList.begin(); iter != projectileList.end(); iter++) //the following for loop iterates over the list of projectiles and destroys each dynamic shape object underneath.
    {
        Shape *sPtr = iter->getShape();
        delete sPtr;
    }

    for (std::list<Shape3D*>::iterator iter = staticSceneGraph.begin(); iter != staticSceneGraph.end(); iter++)
    {
        Shape3D *sPtr = *iter;
        delete sPtr;
    }

    for (std::deque<Shape*>::iterator iter = dynamicSceneGraph.begin(); iter != dynamicSceneGraph.end(); iter++)
    {
        Shape *sPtr = *iter;
        delete sPtr;
    }

    for (std::list<NPC*>::iterator iter = enemyList.begin(); iter != enemyList.end(); iter++)
    {
        Shape3D *sPtr = (*iter)->getShape();
        Path *pPtr = (*iter)->getPath();
        NPC *npcPtr = *iter;

        delete sPtr;
        delete pPtr;
        delete npcPtr;
    }

    for (std::list<NPC*>::iterator iter = friendList.begin(); iter != friendList.begin(); iter++)
    {
        Shape3D *sPtr = (*iter)->getShape();
        Path *pPtr = (*iter)->getPath();
        NPC *npcPtr = *iter;

        delete sPtr;
        delete pPtr;
        delete npcPtr;      
    }

    delete playerCam;

    delete light0;
    delete light1;
    delete light2;
}

static void gl_error(const char *msg)  //SCREAMS INTERNALLY
{
    perror(msg);
    atexit(gl_cleanup);
    exit(-1);
}

void updateFriendlies()
{
    if (friendlySpeed < -1.0)
    {
        friendlySpeed -= FRIENDLY_ACCEL_RATE;
    }
    else if (friendlySpeed > 1.0)
    {
        friendlySpeed -= FRIENDLY_DECEL_RATE;
    }


    //****************************************
    //TEMPORARY GHETTO HACK TO FIX SEGFAULTING
    bool notFinalZone;
    if (zoneIterator != areaList.end())
    {
        notFinalZone = true;
    }
    else
    {
        notFinalZone = false;
    }
    //****************************************
    //****************************************  


    for(std::list<NPC*>::iterator iter = friendList.begin(); iter != friendList.end(); iter++)
    {   
        if ((*iter)->isDead == 0)
        {
            GLfloat *playerPos = player1->ship->getPosition();
            GLfloat *npcPos = (*iter)->getShape()->getPosition();
            GLfloat npcDistance = Vertex(npcPos).getDistance(playerPos);

            if (npcDistance > 30.0)
            {
                friendlySpeed = -0.04;          
            }
            else if (npcDistance > 4.0)
            {   
                int speedFlag = 0;
                if (npcPos[2] < playerPos[2]) //if friendly is ahead of player
                {
                    //npc decelerate:
                    (*iter)->update(
                                    (friendlySpeed += (FRIENDLY_DECEL_RATE + playerProximityBoost)),
                                    &projectileList,
                                    playerPos,
                                    &enemies_killed,
                                    &(zoneIterator->powerUpList),
                                    &enemyList,
                                    &friendList,
                                    notFinalZone
                                   );

                    //printf("Distance: <%g> \nSpeed: <%g> \nproximity boost: <%g>\n", Vertex(npcPos).getDistance(playerPos), friendlySpeed, playerProximityBoost);
                    speedFlag = 1;
                }
                else if(npcPos[2] > playerPos[2]) //if friendly is behind player
                {
                    //npc accelerate
                    (*iter)->update(
                                    (friendlySpeed -= (FRIENDLY_ACCEL_RATE + playerProximityBoost)),
                                    &projectileList,
                                    playerPos,
                                    &enemies_killed,
                                    &(zoneIterator->powerUpList),
                                    &enemyList,
                                    &friendList,
                                    notFinalZone
                                   );
                    speedFlag = 1;                              
                }

                if (npcPos[1] < playerPos[1]) //if friendly is higher than the player
                {
                    //printf("INCREASING ALTITUDE!\n");
                    (*iter)->adjustAltitude(FRIENDLY_ALTITUDE_INCREASE_RATE - playerProximityBoost);
                }
                else if (npcPos[1] > playerPos[1])
                {
                    //printf("DECREASING ALTITUDE!\n");
                    (*iter)->adjustAltitude(FRIENDLY_ALTITUDE_DECREASE_RATE + playerProximityBoost);
                }

                if (npcPos[0] < playerPos[0])
                {
                    (*iter)->adjustBearing(FRIENDLY_X_DECREASE_RATE + playerProximityBoost);
                }
                else if (npcPos[0] > playerPos[0])
                {
                    (*iter)->adjustBearing(FRIENDLY_X_INCREASE_RATE - playerProximityBoost);
                }

                //printf("moving friendly object %d with a speed of %g\n", (int)std::distance(friendList.begin(), iter), friendlySpeed);
                if (speedFlag == 0)
                {
                    (*iter)->update(
                                    friendlySpeed,
                                    &projectileList,
                                    playerPos,
                                    &enemies_killed,
                                    &(zoneIterator->powerUpList),
                                    &enemyList,
                                    &friendList,
                                    notFinalZone
                                   );       
                }

                if (playerProximityBoost > 0.0)
                {
                    if (boostFlag > 5)
                    {
                        playerProximityBoost = 0.0; //reset proximity boost
                        boostFlag = 0;
                    }
                    else
                    {
                        boostFlag++;    
                    }
                }                                                           
            }
            else
            {
                playerProximityBoost = 0.02;
                if (npcPos[1] < playerPos[1]) //if friendly is higher than the player
                {
                    //printf("INCREASING ALTITUDE!\n");
                    (*iter)->adjustAltitude(-0.08);
                }
                else if (npcPos[1] > playerPos[1])
                {
                    //printf("DECREASING ALTITUDE!\n");
                    (*iter)->adjustAltitude(0.08);
                }

                if (npcPos[0] < playerPos[0])
                {
                    (*iter)->adjustBearing(0.08);
                }
                else if (npcPos[0] > playerPos[0])
                {
                    (*iter)->adjustBearing(-0.08);
                }

                (*iter)->update(
                                friendlySpeed,
                                &projectileList,
                                playerPos,
                                &enemies_killed,
                                &(zoneIterator->powerUpList),
                                &enemyList,
                                &friendList,
                                notFinalZone
                               );
            }

            delete[] playerPos;
            delete[] npcPos;
        }
    }
}

void checkTriggers() 
{
    int iterFlag = 0;
    if (zoneCounter != 0)
    {
        GLfloat *playerPos = player1->ship->getPosition();

        if (zoneIterator!=areaList.begin()) 
        {
            iterFlag = 1;
            zoneIterator--; //temporarily decrement zone pointer
        }

        for(std::deque<Spawn>::iterator iter = zoneIterator->NPCList.begin(); iter != zoneIterator->NPCList.end(); iter++) //checks each NPC in the current zone
        {
            if (iter->offset.getDistance(playerPos) < NPC_TRIGGER_RANGE && iter->target->spawnLock != 1) //if the distance to the trigger point is within range
            {
                GLfloat xOffset = 10.0;
                GLfloat yOffset = 10.0;

                int xRand = rand() % 2;
                int yRand = rand() % 2;
                yRand=0; //disables random y positioning - it looks like shit

                if (xRand==1) xOffset*=-1;
                if (yRand==1) yOffset*=-1;

                iter->offset.x+=xOffset;
                iter->offset.y+=yOffset;
                iter->offset.z-=10.0;

                iter->target->local_translate(iter->offset.x, iter->offset.y, iter->offset.z); //translate the ship to location indicated by offset

                //calculate path layout:
                Vertex *waypointList = new Vertex[3];
                if (xRand==1 && yRand==1) waypointList[0] = Vertex(
                                                                   iter->offset.x + 10.0, 
                                                                   iter->offset.y + 10.0, 
                                                                   iter->offset.z + 25.0
                                                                  );

                else if (xRand==1) waypointList[0] = Vertex(
                                                            iter->offset.x + 10.0, 
                                                            iter->offset.y - 10.0, 
                                                            iter->offset.z + 25.0
                                                           );

                else if (yRand==1) waypointList[0] = Vertex(
                                                            iter->offset.x - 10.0, 
                                                            iter->offset.y + 10.0, 
                                                            iter->offset.z + 25.0
                                                           );

                else waypointList[0] = Vertex(
                                              iter->offset.x-10.0,
                                              iter->offset.y-10.0, 
                                              iter->offset.z + 25.0
                                             );

                waypointList[1] = Vertex(-10, 5, 180);
                waypointList[2] = Vertex(-10, 5, 190); 

                Path *npcPath = new Path(waypointList, 3); //create path    

                NPC *newNPC = new NPC(NPC_HOSTILE, AI_DUMB, iter->target, npcPath); //create NPC    

                iter->target->spawnLock = 1; //marks NPC as being spawned

                enemyList.push_front(newNPC);

                delete[] waypointList;
            }
        }

        if (iterFlag==1) zoneIterator++; //return zone iterator to original position

        delete[] playerPos;
    }
}

void drawStaticObjects() 
{
    for (std::list<Shape3D*>::iterator iter = staticSceneGraph.begin(); iter != staticSceneGraph.end(); iter++) //iterates over the general object list and calls draw() on each one
    {
        (*iter)->draw(1);
    }

    glDisable(GL_LIGHTING);

    for (std::deque<Shape*>::iterator iter = dynamicSceneGraph.begin(); iter!= dynamicSceneGraph.end(); iter++) //iterates over temp objects
    {
        (*iter)->draw(GL_QUADS);
    }

    //DRAWS SURFACE:
    glBegin(GL_QUADS);
    glColor3fv(GREEN);
    glVertex4f(-GL_FAR_CLIPPING_PLANE, -10.0, GL_FAR_CLIPPING_PLANE, 1.0);
    glVertex4f(GL_FAR_CLIPPING_PLANE, -10.0, GL_FAR_CLIPPING_PLANE, 1.0);
    glVertex4f(GL_FAR_CLIPPING_PLANE, -10.0, -GL_FAR_CLIPPING_PLANE, 1.0);
    glVertex4f(-GL_FAR_CLIPPING_PLANE, -10.0, -GL_FAR_CLIPPING_PLANE, 1.0);
    glEnd();

    glEnable(GL_LIGHTING);
}

void updatePowerUps()
{
    GLfloat *playerPosition = player1->ship->getPosition();
    for (std::deque<PowerUp*>::iterator iter = zoneIterator->powerUpList.begin(); iter != zoneIterator->powerUpList.end(); iter++)
    {
        (*iter)->update(
                        playerPosition, 
                        &(player1->health), 
                        &(player1->boost)
                       );
    }
    delete[] playerPosition;
}

void drawBillboards()
{
    glDisable(GL_LIGHTING);
    for (std::deque<Billboard*>::iterator iter = billboardList.begin(); iter != billboardList.end(); iter++) //iterates over the general object list and calls draw() on each one
    {
        (*iter)->draw();
    }
    glEnable(GL_LIGHTING);
}

void animateProjectiles()
{
    for (std::deque<Projectile>::iterator iter = projectileList.begin(); iter != projectileList.end(); iter++)
    {
        if(!PAUSED)
        {
            GLfloat playerPos[3] = {
                                    player1->ship->local_p[0], 
                                    player1->ship->local_p[1], 
                                    player1->ship->local_p[2]
                                   };
        
            iter->update(
                         &enemyList, 
                         playerPos, 
                         &(player1->health),
                         &friendList
                        );
        }
        else
        {
            iter->target->draw(GL_QUADS);
        }
    }

/*
    std::transform(
                   projectileList.begin(), projectileList.end(), projectileList.begin(), [](Projectile iter) mutable -> Projectile
                   {
                       if (!PAUSED)
                       {
                           GLfloat playerPos[3] = {
                                                   player1->ship->local_p[0], 
                                                   player1->ship->local_p[1], 
                                                   player1->ship->local_p[2]
                                                  };
                           iter.update(
                                       &enemyList, 
                                       playerPos, 
                                       &(player1->health),
                                       &friendList
                                      );                       
                        }
                        else
                        {
                            iter.target->draw(GL_QUADS);
                        }    

                        return iter;
                    }
                   );
*/

}

void moveCamera(GLfloat xSpeed, GLfloat ySpeed, GLfloat zSpeed)
{
    playerCam->translateCamera(xSpeed, ySpeed, zSpeed);
    cam_x += xSpeed;
    cam_y += ySpeed;
    cam_z += zSpeed;
}

void turnCamera(GLfloat deg, int x, int y, int z)
{
    playerCam->rotateCameraLocalized(deg, x, y, z);
}

void spawnEnemy(GLdouble *loc) //Note: MULTIPLE NPC OBJECTS CAN NOT SHARE THE SAME PATH OR MODEL/SHAPE; each must be distinct to avoid crash/memleak on exit.
{
    Shape3D *hostile0_obj;
    char mesh_file[50];
    strcpy(mesh_file, "arwing.txt");
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    hostile0_obj->local_translate(loc[0]+5, loc[1]+5, loc[2]-50);

    Path *hostile0_path = new Path(Vertex(loc[0]-10, loc[1]-10, loc[2]-5), Vertex(loc[0]-10, loc[1], loc[2]));
    NPC *hostile0 = new NPC(NPC_HOSTILE, AI_DUMB, hostile0_obj, hostile0_path);
    enemyList.push_front(hostile0);

    Shape3D *hostile1_obj;
    char mesh_file0[50];
    strcpy(mesh_file0, "arwing.txt");
    hostile1_obj = new Mesh();
    hostile1_obj->load_mesh(mesh_file0);    
    hostile1_obj->local_translate(loc[0]+10, loc[1]+10, loc[2]-54); 

    Path *hostile1_path = new Path(Vertex(loc[0]-10, loc[1]-10, loc[2]-5), Vertex(loc[0]-10, loc[1], loc[2]));
    NPC *hostile1 = new NPC(NPC_HOSTILE, AI_DUMB, hostile1_obj, hostile1_path);

    enemyList.push_front(hostile1);
}

void NPCupdate()
{


    //****************************************
    //TEMPORARY GHETTO HACK TO FIX SEGFAULTING  
    bool notFinalZone;
    if (zoneIterator != areaList.end())
    {
        notFinalZone = true;
    }
    else
    {
        notFinalZone = false;
    }
    //****************************************
    //****************************************  


    GLfloat *playerPos = player1->ship->getPosition();

    for (std::list<NPC*>::iterator iter = enemyList.begin(); iter != enemyList.end(); iter++)
    {
        if (PAUSED)
        {
            if((*iter)->isDead == 1)
            {
                (*iter)->targetShape->draw(1);
            }
            else
            {
                (*iter)->targetShape->draw_mesh(GL_TRIANGLES, GL_FLAT);
            }
        }
        else
        {
            (*iter)->update(
                            .4, 
                            &projectileList, 
                            playerPos, 
                            &enemies_killed,
                            &(zoneIterator->powerUpList),
                            &enemyList,
                            &friendList,
                            notFinalZone
                           );
        }
    }   

    delete[] playerPos;
}

void animateThrusters() //draw "flames" on back end of Arwing
{
    GLfloat playerPos[3];
    playerPos[0] = player1->ship->local_p[0];
    playerPos[1] = player1->ship->local_p[1];
    playerPos[2] = player1->ship->local_p[2];

    float randX = (rand() % 100)/100.0;
    float randY = (rand() % 100)/100.0;
    
    Shape *thruster;
    if (thrustFlag < 3)
    {
        thruster = new Shape(SHAPE_SPHERE, 5.0, 5.0, 1.0, LIGHT_BLUE);
        thruster->tessellate();
        thruster->scale(.15, .15, .6);
        thruster->translate(playerPos[0], playerPos[1]+0.1, playerPos[2]+.7);   
        
        if(dynamicSceneGraph.size() > 0)
        {
            Shape *sPtr = dynamicSceneGraph.back();
            dynamicSceneGraph.pop_back();
            delete sPtr;
        }

        dynamicSceneGraph.push_front(thruster);
        thrustFlag ++;
    }
    else
    {
        thruster = new Shape(SHAPE_SPHERE, 5.0, 5.0, 1.0, WHITE);
        thruster->tessellate();
        thruster->scale(.2, .15, .2);
        thruster->translate(playerPos[0], playerPos[1]+0.1, playerPos[2]+.5);
        
        if(dynamicSceneGraph.size() > 0)
        {
            Shape *sPtr = dynamicSceneGraph.back();
            dynamicSceneGraph.pop_back();
            delete sPtr;
        }

        dynamicSceneGraph.push_front(thruster);
        thrustFlag ++;
    }

    if(thrustFlag >= 6)
    {
        thrustFlag = 0;
    }
}

void realTimeCleanup() //dynamic memory management system
{
    if (projectileList.size() > 55)
    {
        //printf("\n***55 projectile limit exceeded. Initiating automated cleanup subroutine.***\n");
        for (int i = 55; i > 5; i--)
        {
            Shape *sPtr = projectileList.back().getShape();
            projectileList.pop_back();
            delete sPtr;
        }
    }
    if (dynamicSceneGraph.size() > 40) //originally 25
    {
        //printf("\n***25 temp object limit exceeded. Initiating automated cleanup subroutine.***\n");
        for (int i = 40; i > 5; i--)
        {
            Shape *sPtr = dynamicSceneGraph.back();
            dynamicSceneGraph.pop_back();
            delete sPtr;
        }
    }
    for (std::deque<Billboard*>::iterator iter = billboardList.begin(); iter != billboardList.end(); iter++) //iterates over the general object list and calls draw() on each one
    {
        if ((*iter)->local_p[2] > cam_z)
        {
            delete *iter;
        }
    }
}

void playerFire() //weapons code
{
    fireCounter++;
    if (fireCounter == ROF_MEDIUM)
    {
        GLfloat color[3] = {0.7, 0.1, 0.5};

        GLdouble *camPos = playerCam->getPosition(); //get current camera position coordinates.

        //AUTO TARGETING CODE
        int lockCount = 0;
        GLdouble *camLook;
        if (AUTO_TARGET_ON == 1)
        {
            std::list<NPC*>::iterator targetIterator = enemyList.begin();
            while (targetIterator != enemyList.end())
            {
                GLfloat *targetPosition = (*targetIterator)->getShape()->getPosition();
                if ((Vertex(targetPosition).getDistance(camPos) < AUTO_TARGET_RANGE) && (targetPosition[2] < camPos[2]) && (*targetIterator)->getIFF() != NPC_FRIENDLY)
                {
                    //target is close, auto-lock engaged!
                    //printf("ENGAGING AUTO TARGET!\n");
                    lockCount++;

                    camLook = new GLdouble[3];
                    camLook[0] = (GLdouble)targetPosition[0];
                    camLook[1] = (GLdouble)targetPosition[1];
                    camLook[2] = (GLdouble)targetPosition[2];

                    delete[] targetPosition;
                    break;
                }
                delete[] targetPosition;
                targetIterator++;
            }
        }

        if (lockCount==0)
        {
            camLook = playerCam->getLookPoint();
        }

        GLfloat objPos[3];
        objPos[0] = player1->ship->local_p[0];
        objPos[1] = player1->ship->local_p[1];
        objPos[2] = player1->ship->local_p[2];

        Vertex origin = Vertex(
                               camPos[0], 
                               camPos[1], 
                               camPos[2]
                              ); //point of origin

        Vertex look = Vertex(
                             camPos[0] - player1->ship->local_z[0], 
                             camPos[1] + player1->ship->local_z[1], 
                             camPos[2] + player1->ship->local_z[2]
                            ); //location of look

        vec3 fireVec = vec3(look, origin); //calculate resultant vector
        vec3 *tempVec = fireVec.normalize();
        fireVec = vec3(tempVec->x, tempVec->y, tempVec->z);
        delete tempVec; 

        camPos[1]-=2;
        camPos[2]+=4;

        camLook[0]+=(.6*fireVec.x);
        camLook[1]+=(.6*fireVec.y);
        camLook[2]+=(.6*fireVec.z); 
        
        Shape *weapon0 = new Shape(SHAPE_CONE, 4, 4, 1.0, YELLOW);
        weapon0->tessellate();
        weapon0->scale(.2, 1.6, 0.2);
        weapon0->translate(objPos[0] +0.4, objPos[1], objPos[2]); //spawn burst @ current player location
        weapon0->translate(player1->ship->local_x[0], player1->ship->local_x[1], 0);
        weapon0->revolve(-90.0, 0, 0, 1);

        Shape *weapon1 = new Shape(SHAPE_CONE, 4, 4, 1.0, YELLOW);
        weapon1->tessellate();
        weapon1->scale(.2, 1.6, 0.2);
        weapon1->translate(objPos[0] -0.4, objPos[1], objPos[2]); //spawn burst @ current player location
        weapon1->translate(-player1->ship->local_x[0], -player1->ship->local_x[1], 0);
        weapon1->revolve(-90.0, 0, 0, 1);

        projectileList.push_front(Projectile(LASER_SPEED_SUPER_SLOW, &fireVec, weapon0, GL_FAR_CLIPPING_PLANE, 0)); //we now have the normalized vector giving us the direction of the projectile, as well as the cone to be used. Now, make a Projectile object:
        projectileList.push_front(Projectile(LASER_SPEED_SUPER_SLOW, &fireVec, weapon1, GL_FAR_CLIPPING_PLANE, 0));

        fireFlag = 1;

        delete[] camPos;
        delete[] camLook;

        fireCounter = 0;
    }
}

void createStaticObjects() //called ONCE at the beginning.
{
    //FIRST ZONE:

    Shape3D *tree0;
    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        staticSceneGraph.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        staticSceneGraph.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        staticSceneGraph.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        staticSceneGraph.push_front(tree0); 
    }

    Shape3D *building1 = new Cube(4.0);
    building1->local_scale(5.0, 20.0, 5.0);
    building1->local_translate(-30, -9, 30);
    building1->set_color(GREY_3D);
    staticSceneGraph.push_front(building1); 

    building1 = new Cube(4.0);
    building1->local_scale(5.0, 20.0, 5.0);
    building1->local_translate(30, -9, 30);
    building1->set_color(GREY_3D);
    staticSceneGraph.push_front(building1);                         

    //create additional areas/"levels" with the Zone object to extend the world:
    
    //ZONE 0
    Zone zone0;

    Shape3D *b0 = new Cube(4.0);
    b0->local_translate(60, -9, 0);
    b0->set_color(DARK_GREY_3D);
    zone0.objectList.push_front(b0);

    b0 = new Cube(4.0);
    b0->local_translate(30, -9, -10);
    b0->set_color(DARK_GREY_3D);
    zone0.objectList.push_front(b0);    
    
    b0 = new Cube(4.0);
    b0->local_translate(0, -9, -20);
    b0->set_color(DARK_GREY_3D);
    zone0.objectList.push_front(b0);    

    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone0.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone0.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone0.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone0.objectList.push_front(tree0); 
    }   

    char mesh_file[50];
    strcpy(mesh_file, "arwing.txt");
    Shape3D *hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  

    Spawn enemySpawn; //a Spawn struct for creating NPC's
    enemySpawn.target = hostile_obj;
    
    GLfloat offsetX = 4.0;
    GLfloat offsetY = 0.0;
    GLfloat offsetZ = 100.0;
    enemySpawn.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn_1; //a Spawn struct for creating NPC's
    enemySpawn_1.target = hostile_obj;
    
    offsetX = 0.0;
    offsetY = 2.0;
    offsetZ = 100.0;
    enemySpawn_1.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_1);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn_2; //a Spawn struct for creating NPC's
    enemySpawn_2.target = hostile_obj;
    
    offsetX = -4.0;
    offsetY = 0.0;
    offsetZ = 100.0;
    enemySpawn_2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_2);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn_3; //a Spawn struct for creating NPC's
    enemySpawn_3.target = hostile_obj;
    
    offsetX = 0.0;
    offsetY = -2.0;
    offsetZ = 100.0;
    enemySpawn_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_3);
    
    Shape3D *hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_12; //a Spawn struct for creating NPC's
    enemySpawn_12.target = hostile0_obj;
    
    offsetX = -4.0;
    offsetY = -2.0;
    offsetZ = 25.0;
    enemySpawn_12.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_12);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_4; //a Spawn struct for creating NPC's
    enemySpawn_4.target = hostile0_obj;
    
    offsetX = 2.0;
    offsetY = -2.0;
    offsetZ = 75.0;
    enemySpawn_4.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_4); 
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_5; //a Spawn struct for creating NPC's
    enemySpawn_5.target = hostile0_obj;
    
    offsetX = 0.0;
    offsetY = 3.0;
    offsetZ = 75.0;
    enemySpawn_5.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_5);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_6; //a Spawn struct for creating NPC's
    enemySpawn_6.target = hostile0_obj;
    
    offsetX = -4.0;
    offsetY = 2.0;
    offsetZ = 50.0;
    enemySpawn_6.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_6);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_7; //a Spawn struct for creating NPC's
    enemySpawn_7.target = hostile0_obj;
    
    offsetX = 1.5;
    offsetY = 3.0;
    offsetZ = 50.0;
    enemySpawn_7.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_7);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_8; //a Spawn struct for creating NPC's
    enemySpawn_8.target = hostile0_obj;
    
    offsetX = 4.0;
    offsetY = 2.0;
    offsetZ = 80.0;
    enemySpawn_8.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_8);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_9; //a Spawn struct for creating NPC's
    enemySpawn_9.target = hostile0_obj;
    
    offsetX = -1.0;
    offsetY = -3.0;
    offsetZ = -18.0;
    enemySpawn_9.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_9);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_10; //a Spawn struct for creating NPC's
    enemySpawn_10.target = hostile0_obj;
    
    offsetX = -1.0;
    offsetY = 2.0;
    offsetZ = 130.0;
    enemySpawn_10.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_10);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn_11; //a Spawn struct for creating NPC's
    enemySpawn_11.target = hostile0_obj;
    
    offsetX = 1.5;
    offsetY = 0.0;
    offsetZ = 0.0;
    enemySpawn_11.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone0.NPCList.push_front(enemySpawn_11);
    
    areaList.push_back(zone0); //add this zone to global list of zones
    
    //ZONE 1
    Zone zone1;
    
    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone1.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone1.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone1.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 200);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone1.objectList.push_front(tree0); 
    }               
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0; //a Spawn struct for creating NPC's
    enemySpawn0.target = hostile0_obj;
    
    offsetX = 4.0;
    offsetY = -2.0;
    offsetZ = 25.0;
    enemySpawn0.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0);

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_1; //a Spawn struct for creating NPC's
    enemySpawn0_1.target = hostile0_obj;
    
    offsetX = -2.0;
    offsetY = -2.0;
    offsetZ = 125.0;
    enemySpawn0_1.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_1);    

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_2; //a Spawn struct for creating NPC's
    enemySpawn0_2.target = hostile0_obj;
    
    offsetX = 2.0;
    offsetY = -1.0;
    offsetZ = 125.0;
    enemySpawn0_2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_2);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_3; //a Spawn struct for creating NPC's
    enemySpawn0_3.target = hostile0_obj;
    
    offsetX = -4.0;
    offsetY = -2.0;
    offsetZ = 50.0;
    enemySpawn0_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_3);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_4; //a Spawn struct for creating NPC's
    enemySpawn0_4.target = hostile0_obj;
    
    offsetX = 0.0;
    offsetY = 0.0;
    offsetZ = 50.0;
    enemySpawn0_4.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_4);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_5; //a Spawn struct for creating NPC's
    enemySpawn0_5.target = hostile0_obj;
    
    offsetX = 1.0;
    offsetY = 2.0;
    offsetZ = 100.0;
    enemySpawn0_5.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_5);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_6; //a Spawn struct for creating NPC's
    enemySpawn0_6.target = hostile0_obj;
    
    offsetX = -1.0;
    offsetY = -1.0;
    offsetZ = 120.0;
    enemySpawn0_6.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_6);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_7; //a Spawn struct for creating NPC's
    enemySpawn0_7.target = hostile0_obj;
    
    offsetX = -1.0;
    offsetY = 2.0;
    offsetZ = 120.0;
    enemySpawn0_7.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_7);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn0_8; //a Spawn struct for creating NPC's
    enemySpawn0_8.target = hostile0_obj;
    
    offsetX = 1.5;
    offsetY = 0.0;
    offsetZ = 0.0;
    enemySpawn0_8.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone1.NPCList.push_front(enemySpawn0_8);

    //bunch of big buildings on left and right
    Shape3D *zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(-30, -9, 100);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building);

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(-30, -9, 70);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(-30, -9, 30);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(-30, -9, 0);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(30, -9, 100);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building);

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(30, -9, 70);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(30, -9, 30);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 24.0, 5.0);
    zone1Building->local_translate(30, -9, 0);
    zone1Building->set_color(DARK_GREY_3D);
    zone1.objectList.push_front(zone1Building); 

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 2.0, 5.0);
    zone1Building->local_translate(0, -9, 0);
    zone1Building->set_color(GREY_3D);
    zone1.objectList.push_front(zone1Building);         

    zone1Building = new Cube(4.0);
    zone1Building->local_scale(5.0, 2.0, 5.0);
    zone1Building->local_translate(0, -9, 60);
    zone1Building->set_color(GREY_3D);
    zone1.objectList.push_front(zone1Building);             

    areaList.push_back(zone1);  

    Zone zone2;

    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone2.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone2.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone2.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone2.objectList.push_front(tree0); 
    }       

    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn2; //a Spawn struct for creating NPC's
    enemySpawn2.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn2_1; //a Spawn struct for creating NPC's
    enemySpawn2_1.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_1.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_1);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn2_2; //a Spawn struct for creating NPC's
    enemySpawn2_2.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_2);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn2_3; //a Spawn struct for creating NPC's
    enemySpawn2_3.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_3);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_12; //a Spawn struct for creating NPC's
    enemySpawn2_12.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_12.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_12);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_4; //a Spawn struct for creating NPC's
    enemySpawn2_4.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_4.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_4);    
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_5; //a Spawn struct for creating NPC's
    enemySpawn2_5.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_5.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_5);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_6; //a Spawn struct for creating NPC's
    enemySpawn2_6.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_6.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_6);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_7; //a Spawn struct for creating NPC's
    enemySpawn2_7.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_7.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_7);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_8; //a Spawn struct for creating NPC's
    enemySpawn2_8.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_8.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_8);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_9; //a Spawn struct for creating NPC's
    enemySpawn2_9.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_9.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_9);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_10; //a Spawn struct for creating NPC's
    enemySpawn2_10.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_10.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_10);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn2_11; //a Spawn struct for creating NPC's
    enemySpawn2_11.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn2_11.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone2.NPCList.push_front(enemySpawn2_11);


    //The Zone 2 "City":    

    Shape3D *b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 19.0, 5.0);
    b2_0->local_translate(30, -9, 0);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);  

    //right
    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(30, -9, 100);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);  

    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(30, -9, 80);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);      

    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(30, -9, 60);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);  


    //left:
    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(-30, -9, 140);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);  

    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(-30, -9, 120);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);      

    b2_0 = new Cube(4.0);
    b2_0->local_scale(5.0, 24.0, 5.0);
    b2_0->local_translate(-30, -9, 100);
    b2_0->set_color(DARK_GREY_3D);
    zone2.objectList.push_front(b2_0);      

    //Overpass b/w two buildings
    b2_0 = new Cube(4.0);
    b2_0->local_scale(15.0, 2.0, 2.0);
    b2_0->local_translate(0, 12, 100);
    b2_0->set_color(GREY_3D);
    zone2.objectList.push_front(b2_0);  
    
/*  
    //Cylindrical Building:
    b2_0 = new Cylinder(4.0, 20.0, 10.0, 10.0);
    //b2_0->local_scale(7.0, 20, 7.0);
    b2_0->local_translate(20, -9, -45);
    b2_0->set_color(GREY_3D);
    zone2.objectList.push_front(b2_0);  

    b2_0 = new Cylinder(4.0, 20.0, 10.0, 10.0);
    //b2_0->local_scale(7.0, 20, 7.0);
    b2_0->local_translate(15, -9, -55);
    b2_0->set_color(GREY_3D);
    zone2.objectList.push_front(b2_0);      
*/
    areaList.push_back(zone2); //load the zone into the global structure

    //ZONE 3
    Zone zone3;



    ////////////////////POWER UP TEST///////////////
    Shape *powerupShape;
    powerupShape = new Shape(SHAPE_SPHERE, 15.0, 15.0, 1.0, RED); 
    powerupShape->tessellate();
    //powerupShape->scale(3.0, 3.0, 3.0);
    powerupShape->translate(0.0, 0.0, 150.0);

    PowerUp *pUp0 = new PowerUp(powerupShape, .50, .25);

    zone3.powerUpList.push_front(pUp0);
    ////////////////////////////////////////////////    



    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone3.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone3.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone3.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone3.objectList.push_front(tree0); 
    }
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn3_0; //a Spawn struct for creating NPC's
    enemySpawn3_0.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn3_0.offset = Vertex(-3.0, -2.0, 11.0); //x/y/z starting position
    zone3.NPCList.push_front(enemySpawn3_0); //load into this zone's NPC list

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn3_1; //a Spawn struct for creating NPC's
    enemySpawn3_1.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn3_1.offset = Vertex(4.0, -3.0, 120.0); //x/y/z starting position
    zone3.NPCList.push_front(enemySpawn3_1); //load into this zone's NPC list   

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn3_2; //a Spawn struct for creating NPC's
    enemySpawn3_2.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn3_2.offset = Vertex(-3.0, 5.0, 100.0); //x/y/z starting position
    zone3.NPCList.push_front(enemySpawn3_2); //load into this zone's NPC list
     
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    
    Spawn enemySpawn3_3; //a Spawn struct for creating NPC's
    enemySpawn3_3.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn3_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone3.NPCList.push_front(enemySpawn3_3);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn3_12; //a Spawn struct for creating NPC's
    enemySpawn3_12.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn3_12.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone3.NPCList.push_front(enemySpawn3_12);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn3_4; //a Spawn struct for creating NPC's
    enemySpawn3_4.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn3_4.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone3.NPCList.push_front(enemySpawn3_4);

    areaList.push_back(zone3);


    //ZONE 4
    Zone zone4;

    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone4.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone4.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone4.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone4.objectList.push_front(tree0); 
    }   

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn4_0; //a Spawn struct for creating NPC's
    enemySpawn4_0.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn4_0.offset = Vertex(-3.0, 2.0, 11.0); //x/y/z starting position
    zone4.NPCList.push_front(enemySpawn4_0); //load into this zone's NPC list

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn4_1; //a Spawn struct for creating NPC's
    enemySpawn4_1.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn4_1.offset = Vertex(4.0, 1.0, 120.0); //x/y/z starting position
    zone4.NPCList.push_front(enemySpawn4_1); //load into this zone's NPC list   

    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn4_2; //a Spawn struct for creating NPC's
    enemySpawn4_2.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn4_2.offset = Vertex(-3.0, 1.0, 100.0); //x/y/z starting position
    zone4.NPCList.push_front(enemySpawn4_2); //load into this zone's NPC list   
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn4_3; //a Spawn struct for creating NPC's
    enemySpawn4_3.target = hostile0_obj; //handle to a loaded mesh
    enemySpawn4_3.offset = Vertex(-3.0, -1.0, 200.0); //x/y/z starting position
    zone4.NPCList.push_front(enemySpawn4_2); //load into this zone's NPC list   

    //arch 1:
    Shape3D *b4_0 = new Cube(4.0);
    b4_0->local_scale(10.0, 2.0, 2.0);
    b4_0->local_translate(0, 12, 130);
    b4_0->set_color(GREY_3D);
    zone4.objectList.push_front(b4_0);

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 14.0, 5.0);
    b4_0->local_translate(-20, -9, 130);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 14.0, 5.0);
    b4_0->local_translate(20, -9, 130);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);  

    //arch 2:
    b4_0 = new Cube(4.0);
    b4_0->local_scale(10.0, 2.0, 2.0);
    b4_0->local_translate(0, 12, 100);
    b4_0->set_color(GREY_3D);
    zone4.objectList.push_front(b4_0);      

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 14.0, 5.0);
    b4_0->local_translate(-20, -9, 100);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 14.0, 5.0);
    b4_0->local_translate(20, -9, 100);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);  

    //building to fly over:
    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 2.0, 5.0);
    b4_0->local_translate(0, -9, 0);
    b4_0->set_color(GREY_3D);
    zone4.objectList.push_front(b4_0);  

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 19.0, 5.0);
    b4_0->local_translate(-40, -9, -40.0);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);      

    b4_0 = new Cube(4.0);
    b4_0->local_scale(5.0, 19.0, 5.0);
    b4_0->local_translate(40, -9, -30.0);
    b4_0->set_color(DARK_GREY_3D);
    zone4.objectList.push_front(b4_0);                  

    areaList.push_back(zone4);


    //ZONE 5
    Zone zone5;


    //pipeline 1:
    Shape3D *b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(20, -9.0, 140);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(20, -9.0, 110);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(20, -9.0, 80);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(20, -9.0, 50);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(20, -9.0, 20);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);          

    b5_0 = new Cube(4.0);
    b5_0->local_scale(1.0, 1.0, 45.0);
    b5_0->local_translate(20, 6.0, 110);
    b5_0->set_color(GREY_3D);
    zone5.objectList.push_front(b5_0);  

    //pipeline 2:       

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(-20, -9.0, 140);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(-20, -9.0, 110);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(-20, -9.0, 80);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(-20, -9.0, 50);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);  

    b5_0 = new Cube(4.0);
    b5_0->local_scale(3.0, 8.0, 3.0);
    b5_0->local_translate(-20, -9.0, 20);
    b5_0->set_color(DARK_GREY_3D);
    zone5.objectList.push_front(b5_0);          

    b5_0 = new Cube(4.0);
    b5_0->local_scale(1.0, 1.0, 45.0);
    b5_0->local_translate(-20, 6.0, 110);
    b5_0->set_color(GREY_3D);
    zone5.objectList.push_front(b5_0);      

    b5_0 = new Cube(4.0);
    b5_0->local_scale(20.0, 1.0, 1.0);
    b5_0->local_translate(0, 6.0, 20);
    b5_0->set_color(GREY_3D);
    zone5.objectList.push_front(b5_0);          


    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn5; //a Spawn struct for creating NPC's
    enemySpawn5.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn5_1; //a Spawn struct for creating NPC's
    enemySpawn5_1.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_1.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_1);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn5_2; //a Spawn struct for creating NPC's
    enemySpawn5_2.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_2);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn5_3; //a Spawn struct for creating NPC's
    enemySpawn5_3.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_3);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn5_12; //a Spawn struct for creating NPC's
    enemySpawn5_12.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_12.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_12);
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn5_4; //a Spawn struct for creating NPC's
    enemySpawn5_4.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_4.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_4);    
    
    hostile0_obj = new Mesh();
    hostile0_obj->load_mesh(mesh_file); 
    Spawn enemySpawn5_5; //a Spawn struct for creating NPC's
    enemySpawn5_5.target = hostile0_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn5_5.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone5.NPCList.push_front(enemySpawn5_5);    

    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone5.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone5.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone5.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone5.objectList.push_front(tree0); 
    }   

    areaList.push_back(zone5);



    Zone zone6;

    for (int i = 0; i < (int)(FOLIAGE_DENSITY); i++)
    {
        GLfloat xRand = rand() % 100;
        GLfloat zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone6.objectList.push_front(tree0);
        
        xRand = -1*(rand() % 100);
        zRand = rand() % 200;
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone6.objectList.push_front(tree0); 
        
        xRand = rand() % 100;
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone6.objectList.push_front(tree0);             
        
        xRand = -1*(rand() % 100);
        zRand = -1*(rand() % 150);
        tree0 = new Cone(1.0, 4.0, 4, 10);
        tree0->local_translate(xRand, -9, zRand);
        tree0->set_color4f(0.0, 0.3, 0.0, 1.0);
        
        zone6.objectList.push_front(tree0); 
    }       

    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn6_1; //a Spawn struct for creating NPC's
    enemySpawn6_1.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn6_1.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone6.NPCList.push_front(enemySpawn6_1);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn6_2; //a Spawn struct for creating NPC's
    enemySpawn6_2.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn6_2.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone6.NPCList.push_front(enemySpawn6_2);
    
    hostile_obj = new Mesh();
    hostile_obj->load_mesh(mesh_file);  
    
    Spawn enemySpawn6_3; //a Spawn struct for creating NPC's
    enemySpawn6_3.target = hostile_obj;
    
    offsetX = (rand() % 8) - 4.0;
    offsetY = (rand() % 8) - 4.0;
    offsetZ = (rand() % 180) - 50.0;
    enemySpawn6_3.offset = Vertex(offsetX, offsetY, offsetZ);
    
    zone6.NPCList.push_front(enemySpawn6_3);

    areaList.push_back(zone6);  
    
    zoneIterator = areaList.begin();
    //falling out of void...all that remains are the dynamic objects above, which are being kept track of via global data structures for eventual deletion. 
}

void worldRedraw() //called several times through each level; deletes all scene objects whose Z-value is > the current playerlocation. VERY EXPENSIVE!! Should only be called a few times.
{
    //printf("\nLoading...\n");
    zoneCounter++;
    //STEP 1: erase world:
    cout << "No. of Static Objects:  " << staticSceneGraph.size() << endl;
    while (enemyList.size() != 0) //delete npc's
    {
        Shape3D *sPtr = enemyList.back()->getShape();
        Path *pPtr = enemyList.back()->getPath();
        NPC *npcPtr = enemyList.back();
        
        enemyList.pop_back();
        
        delete sPtr;
        delete pPtr;
        delete npcPtr;
    }
    
    while (dynamicSceneGraph.size() != 0) //delete particles/effects etc...
    {
        Shape *sPtr = dynamicSceneGraph.back();
        dynamicSceneGraph.pop_back();
        delete sPtr;
    }
    
    std::list<Shape3D*>::iterator iter = staticSceneGraph.begin();
    while (iter != staticSceneGraph.end()) //selectively delete only items that are BEHIND the player
    {
        GLfloat *objPos = (*iter)->getPosition();
        //if (objPos[2] > -150.0)
        //{
            Shape3D *shpPtr = *iter; //explicitly acquire pointer in a variable.
            iter = staticSceneGraph.erase(iter); //erase the pointer entry in the scenegraph.
            delete shpPtr;  //free memory.  
            iter = staticSceneGraph.begin(); //set iterator back to beginning
        //}
        delete[] objPos;
        iter++;
    }
    
    //STEP 2: build next zone
    for(std::deque<Shape3D*>::iterator iter = zoneIterator->objectList.begin(); iter != zoneIterator->objectList.end(); iter++)
    {
        staticSceneGraph.push_front(*iter); //transfer current zone's world objects into currently active scenegraph
    }
    
    //STEP 3: warp player back to start
    playerCam->synchronizedTranslate(0.0, 0.0, 250.0, player1->ship);

    //STEP 4: warp friendly(s) back to start
    for (std::list<NPC*>::iterator iter = friendList.begin(); iter != friendList.end(); iter++)
    {
        GLfloat *friendlyPosition = (*iter)->getShape()->getPosition();
        (*iter)->getShape()->world_translate(friendlyPosition[0], friendlyPosition[1], 250.0);
        delete[] friendlyPosition;
    }
    
    //STEP 5: increment global zone pointer
    if (zoneIterator != areaList.end())
    {
        zoneIterator++; 
    }
    else
    {
        zoneIterator--;
        game_status=GAME_WON;
        //end the game.
        printf("\n_____________***GAME OVER!***_____________\n");
        printf("***NEW HIGH SCORE!***\n");
        printf("Kills: \n");
        printf("Deaths: \n");
        printf("Obstacles: \n");
        //atexit(gl_cleanup); 
        //exit(0);      
    }
    
}
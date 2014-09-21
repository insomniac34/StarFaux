//Written by Tyler Raborn & Charlie Koch

extern float PLAYER_WEAPON_DAMAGE;
extern float NPC_WEAPON_DAMAGE;

//openGL definitions 
#define GL_WINDOW_WIDTH 1200 //i figured 800x600 is a good start point feel free to mess with any of these values -tyler
#define GL_WINDOW_HEIGHT 800

#define GL_FOV 60
#define GL_ASPECT_RATIO 1.5
#define GL_NEAR_CLIPPING_PLANE 1
#define GL_FAR_CLIPPING_PLANE 200.0

//game mechanics
#define LASER_SPEED_SUPER_SLOW 2.0
#define LASER_SPEED_SLOW 10.0
#define LASER_SPEED_MEDIUM 20.0
#define LASER_SPEED_FAST 30.0

#define TURN_RATE 5
#define PI 3.14159265359
#define RADS 0.0174532
#define MAX_PROJECTILES 25
#define MAX_STATIC_OBJECTS 50
#define DEFAULT_SPEED -0.4

#define FOLIAGE_DENSITY 100
#define FOLIAGE_DENSITY_LOW 40

#define AUTO_TARGET_RANGE 20.0
#define AUTO_TARGET_ON 1

// game states
#define GAME_START 0
#define GAME_ON 1
#define GAME_OVER 2
#define GAME_WON 3

#define SHAPE_CUBE 0
#define SHAPE_CYLINDER 1
#define SHAPE_SPHERE 2
#define SHAPE_TORUS 3
#define SHAPE_CONE 4

#define NPC_HOSTILE 0
#define NPC_NEUTRAL 1
#define NPC_FRIENDLY 2			

//rate of fire				
#define ROF_SUPER_LOW 10
#define ROF_LOW 8
#define ROF_MEDIUM 6
#define ROF_HIGH 4
#define ROF_SUPER_HIGH 2

#define NPC_TRIGGER_RANGE 75.0 //how far you have to be from spawnpoint to trigger enemy 

//AI:
#define AI_DUMB 3
#define AI_LIEUTENANT 2
#define AI_COLONEL 1
#define AI_GENERAL 0

//AI movement speed definitions:
#define FRIENDLY_DECEL_RATE 0.002 //was 0.002
#define FRIENDLY_ACCEL_RATE 0.005 //was 0.005
#define FRIENDLY_ALTITUDE_INCREASE_RATE 0.02
#define FRIENDLY_ALTITUDE_DECREASE_RATE -0.02
#define FRIENDLY_X_INCREASE_RATE 0.02
#define FRIENDLY_X_DECREASE_RATE -0.02

//DIFFICULTY:
#define DIFFICULTY_LEVEL 1 //0 = easy
                           //1 = medium
                           //2 = hard
                           //3 = insane

//POWERUPS:
#define POWERUP_HEALTH_BOOST .25

#define POWERUP_DROP_FREQ_LOW 4 //25% of all kills yield a powerup
#define POWERUP_DROP_FREQ_MED 3 //33% of all kills yield a powerup
#define POWERUP_DROP_FREQ_HIGH 2 //50% of all kills yield a powerup

#define POWERUP_SHIFT_AMT_X 0.0
#define POWERUP_SHIFT_AMT_Y 0.05
#define POWERUP_SHIFT_AMT_Z 0.0

//#define ACTIVATE_CUTSCENE 0
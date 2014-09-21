/****************************************************
 FILE:		player.h
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		December 2, 2013
 DESC:		Header for the Player Character.
 ****************************************************/

#include <cmath>
#include "Menu.h"
#include "Shape3D.h"

#define UP_BUTTON 0
#define DOWN_BUTTON 1
#define LEFT_BUTTON 2
#define RIGHT_BUTTON 3
#define ROLL_L_BUTTON 4
#define ROLL_R_BUTTON 5
#define BOOST_BUTTON 6
#define BRAKE_BUTTON 7
#define FIRE_BUTTON 8
#define PAUSE_BUTTON 9

#define BR_WINDOW 8

extern int FIRE_LASERS;
extern int INCREASE_SPEED;
extern int DECREASE_SPEED;

class player
{	
public:
	float max_pitch, max_roll;
	float pitch_interval, roll_interval, roll_boost;
	float barrel_interval, barrel_angle;
	float max_move, camera_move, move_interval;
	
	typedef struct button
	{
		int is_down;
		int timer;
	}button;
	
	Shape3D* ship;
	button gamepad[10];
	int barrel_roll;
	
	float health;
	float boost;
	
	player();
	~player();
	
	void update(float origin[3]);
	void update_input();
	void update_rotation();
	void keyPressed(unsigned char key);
	void keyReleased(unsigned char key);
};
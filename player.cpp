/****************************************************
 FILE:		player.cpp
 PLATFORM:	Mac OS X
 AUTHOR:		Charles Koch
 DATE:		December 2, 2013
 DESC:		Class for the Player Character.
 ****************************************************/

#include "player.h"

player :: player()
{
	char mesh_file[50];
	strcpy(mesh_file, "arwing.txt");
	// initialize mesh
	ship = new Mesh();
	// load mesh file
	ship->load_mesh(mesh_file);
	// rotate ship to point forward
	ship->local_rotate_y(M_PI);
	
	gamepad[UP_BUTTON].is_down = 0;
	gamepad[UP_BUTTON].timer = 0;
	gamepad[DOWN_BUTTON].is_down = 0;
	gamepad[DOWN_BUTTON].timer = 0;
	gamepad[LEFT_BUTTON].is_down = 0;
	gamepad[LEFT_BUTTON].timer = 0;
	gamepad[RIGHT_BUTTON].is_down = 0;
	gamepad[RIGHT_BUTTON].timer = 0;
	gamepad[ROLL_L_BUTTON].is_down = 0;
	gamepad[ROLL_L_BUTTON].timer = 0;
	gamepad[ROLL_R_BUTTON].is_down = 0;
	gamepad[ROLL_R_BUTTON].timer = 0;
	gamepad[BOOST_BUTTON].is_down = 0;
	gamepad[BOOST_BUTTON].timer = 0;
	gamepad[BRAKE_BUTTON].is_down = 0;
	gamepad[BRAKE_BUTTON].timer = 0;
	gamepad[FIRE_BUTTON].is_down = 0;
	gamepad[FIRE_BUTTON].timer = 0;
	gamepad[PAUSE_BUTTON].is_down = 0;
	gamepad[PAUSE_BUTTON].timer = 0;
	
	barrel_roll = 0;
	
	max_pitch = M_PI/25.0;
	max_roll = 2*M_PI/25.0;
	pitch_interval = 1;
	roll_interval = M_PI/75.0;
	roll_boost = 0.07;
	barrel_angle = 0;
	barrel_roll = 0;
	barrel_interval = M_PI/20;
	move_interval = 0.09;
	camera_move = 2;
	max_move = 4;
	
	health = 1.0;
	boost = 1.0;
	
}

void player :: update(float origin[3])
{
	update_input();
	// check input
	
	//update_rotation();
	
	//  UP BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[UP_BUTTON].is_down > 0)
	{
		
		// move it in space
		float move = move_interval;
		if(ship->local_p[1] < origin[1] + max_move)
		{
			ship->world_translate(0, move, 0);
		}
	}
	else
	{
		
	}

	//  DOWN BUTTON PRESSED
	/////////////////////////////////////////////////////

	if (gamepad[DOWN_BUTTON].is_down > 0)
	{
		
		// move it in space
		float move = -move_interval;
		if(ship->local_p[1] > origin[1] - max_move)
		{
			ship->world_translate(0, move, 0);
		}
	}
	else
	{
			
	}
	
	//  Y-AXIS CORRECTION
	/////////////////////////////////////////////////////
	
	if( ((gamepad[ROLL_L_BUTTON].is_down > 0 || gamepad[ROLL_R_BUTTON].is_down > 0) && (gamepad[UP_BUTTON].is_down == 0 && gamepad[DOWN_BUTTON].is_down == 0)) || (gamepad[ROLL_L_BUTTON].is_down == 0 && gamepad[ROLL_R_BUTTON].is_down == 0) )
	{
		float pitch;
		
		if(ship->rotation[1] > 0)
		{
			pitch = -pitch_interval;
			if(ship->rotation[1] + pitch >= 0)
			{
				ship->rotation[1] += pitch;
				ship->local_rotate_y(pitch);
			}
			else
			{
				pitch = -1*(ship->rotation[1] + pitch);
				ship->rotation[1] += pitch;
				ship->local_rotate_y(pitch);
			}
		}
		else if(ship->rotation[1] < 0)
		{
			pitch = pitch_interval;
			if(ship->rotation[1] + pitch <= 0)
			{
				ship->rotation[1] += pitch;
				ship->local_rotate_y(pitch);
			}
			else
			{
				pitch = -1*(ship->rotation[1] + pitch);
				ship->rotation[1] += pitch;
				ship->local_rotate_y(pitch);
			}
		}
	}
	
	//  RIGHT BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[RIGHT_BUTTON].is_down > 0)
	{
		// roll the ship
		if (gamepad[ROLL_L_BUTTON].is_down == 0 && barrel_roll == 0)
		{
			float roll = roll_interval;
			if(ship->rotation[2] < max_roll)
			{
				if(ship->rotation[2] + roll <= max_roll)
				{
					ship->rotation[2] += roll;
					ship->local_rotate_z(roll);
				}
			}
		}
		// move it in space
		float move;
		if(gamepad[ROLL_R_BUTTON].is_down > 0)
		{
			move = move_interval + roll_boost;
		}
		else
		{
			move = move_interval;
		}
		if(ship->local_p[0] < origin[0] + max_move)
		{
			ship->world_translate(move, 0, 0);
		}
	}
	else if (gamepad[ROLL_R_BUTTON].is_down == 0 && gamepad[ROLL_L_BUTTON].is_down == 0 && barrel_roll == 0 && (ship->rotation[2] > 0 || ship->rotation[2] > 2*M_PI))
	{
		float roll = -roll_interval;
		if(ship->rotation[2] > 0)
		{
			if(ship->rotation[2] + roll >= 0 || ship->rotation[2] + roll >= -2*M_PI)
			{
				ship->rotation[2] += roll;
				ship->local_rotate_z(roll);
			}
			else
			{
				roll = -1*(ship->rotation[2] + roll);
				ship->rotation[2] += roll;
				ship->local_rotate_z(roll);
			}
		}
	}
	
	//  LEFT BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[LEFT_BUTTON].is_down > 0)
	{
		// roll the ship
		if( gamepad[ROLL_R_BUTTON].is_down == 0 && barrel_roll == 0 )
		{
			float roll = -roll_interval;
			if(ship->rotation[2] > -max_roll)
			{
				if(ship->rotation[2] + roll >= -max_roll)
				{
					ship->rotation[2] += roll;
					ship->local_rotate_z(roll);
				}
			}
		}
		// move it in space
		float move;
		if(gamepad[ROLL_L_BUTTON].is_down > 0)
		{
			move = -move_interval - roll_boost;
		}
		else
		{
			move = -move_interval;
		}
		if(ship->local_p[0] > origin[0] - max_move)
		{
			ship->world_translate(move, 0, 0);
		}
	}
	else if (gamepad[ROLL_R_BUTTON].is_down == 0 && gamepad[ROLL_L_BUTTON].is_down == 0 && barrel_roll == 0 && ship->rotation[2] < 0)
	{
		float roll = roll_interval;
		if(ship->rotation[2] < 0)
		{
			if(ship->rotation[2] + roll <= 0 || ship->rotation[2] + roll <= 2*M_PI)
			{
				ship->rotation[2] += roll;
				ship->local_rotate_z(roll);
			}
			else
			{
				roll = -1*(ship->rotation[2] + roll);
				ship->rotation[2] += roll;
				ship->local_rotate_z(roll);
			}
		}
	}
	
	//  ROLL LEFT BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[ROLL_L_BUTTON].is_down > 0)
	{
		if (gamepad[ROLL_L_BUTTON].timer < BR_WINDOW && barrel_roll == 0)
		{
			barrel_roll = -1;
		}
		else
		{
			// roll the ship
			float roll = -barrel_interval;
			if(ship->rotation[2] > -M_PI/2)
			{
				if(ship->rotation[2] + roll >= -M_PI/2)
				{
					ship->rotation[2] += roll;
					ship->local_rotate_z(roll);
				}
			}
		}
	}
	
	//  ROLL RIGHT BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[ROLL_R_BUTTON].is_down > 0)
	{
		if (gamepad[ROLL_R_BUTTON].timer < BR_WINDOW && barrel_roll == 0)
		{
			barrel_roll = 1;
		}
		else
		{
			// roll the ship
			float roll = barrel_interval;
			if(ship->rotation[2] < M_PI/2)
			{
				if(ship->rotation[2] + roll <= M_PI/2)
				{
					ship->rotation[2] += roll;
					ship->local_rotate_z(roll);
				}
			}
		}
	}
	
	//  BARREL ROLL ANIMATION
	/////////////////////////////////////////////////////
	
	if ( barrel_roll != 0 )
	{
		float roll;
		if ( barrel_roll > 0 )
		{
			roll = barrel_interval;
		}
		else
		{
			roll = -barrel_interval;
		}
		barrel_angle += roll;
		ship->rotation[2] += roll;
		ship->local_rotate_z(roll);
		if (barrel_angle >= 2*M_PI || barrel_angle <= -2*M_PI)
		{
			if( barrel_roll > 0)
			{
				ship->rotation[2] = ship->rotation[2] - (2*M_PI);
			}
			else
			{
				ship->rotation[2] = ship->rotation[2] + (2*M_PI);
			}
			barrel_roll = 0;
			barrel_angle = 0;
		}
	}
	
	//  BOOST BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[BOOST_BUTTON].is_down > 0 && gamepad[BOOST_BUTTON].timer == 0)
	{
		if(boost > 0)
		{
			INCREASE_SPEED = 1;
			boost -= 0.04;
		}
		else
		{
			INCREASE_SPEED = 0;
			gamepad[BOOST_BUTTON].timer = 1;
		}
	}
	else
	{
		if(boost < 1)
		{
			boost += 0.005;
		}
	}
	
	//  BRAKE BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[BRAKE_BUTTON].is_down > 0)
	{
		if(boost > 0 && gamepad[BRAKE_BUTTON].timer == 0)
		{
			DECREASE_SPEED = 1;
			boost -= 0.04;
		}
		else
		{
			DECREASE_SPEED = 0;
			gamepad[BRAKE_BUTTON].timer = 1;
		}
	}
	else
	{
		if(boost < 1)
		{
			boost += 0.005;
		}
	}
	
	//  FIRE BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[FIRE_BUTTON].is_down > 0)
	{
		FIRE_LASERS = 1;
	}
	
	//  PAUSE BUTTON PRESSED
	/////////////////////////////////////////////////////
	
	if (gamepad[PAUSE_BUTTON].is_down > 0 && gamepad[PAUSE_BUTTON].timer == 0)
	{
		//PAUSED == true;
		gamepad[PAUSE_BUTTON].timer = 1;
	}
	// update mesh
	ship->update();
}

void player :: update_input()
{
	if(gamepad[ROLL_L_BUTTON].is_down == 0)
	{
		if(gamepad[ROLL_L_BUTTON].timer < BR_WINDOW)
		{
			gamepad[ROLL_L_BUTTON].timer++;
		}
	}

	if(gamepad[ROLL_R_BUTTON].is_down == 0)
	{
		if(gamepad[ROLL_R_BUTTON].timer < BR_WINDOW)
		{
			gamepad[ROLL_R_BUTTON].timer++;
		}
	}
}

void player :: keyPressed(unsigned char key)
{
	switch(key)
	{
		case 'i':
			gamepad[UP_BUTTON].is_down = 1;
			menu_press_up();
			break;
		case 'k':
			gamepad[DOWN_BUTTON].is_down = 1;
			menu_press_down();
			break;
		case 'j':
			gamepad[LEFT_BUTTON].is_down = 1;
			break;
		case 'l':
			gamepad[RIGHT_BUTTON].is_down = 1;
			break;
		case 'a':
			gamepad[ROLL_L_BUTTON].is_down = 1;
			break;
		case 'd':
			gamepad[ROLL_R_BUTTON].is_down = 1;
			break;
		case 'w':
			gamepad[BOOST_BUTTON].is_down = 1;
			break;
		case 's':
			gamepad[BRAKE_BUTTON].is_down = 1;
			menu_confirm();
			break;
		case ' ':
			gamepad[FIRE_BUTTON].is_down = 1;
			menu_confirm();
			break;
		case 'p':
			gamepad[PAUSE_BUTTON].is_down = 1;
			gamepad[PAUSE_BUTTON].timer = 1;
			//menu_confirm();
			break;
	}
}

void player :: keyReleased(unsigned char key)
{
	switch(key)
	{
		case 'i':
			gamepad[UP_BUTTON].is_down = 0;
			break;
		case 'k':
			gamepad[DOWN_BUTTON].is_down = 0;
			break;
		case 'j':
			gamepad[LEFT_BUTTON].is_down = 0;
			break;
		case 'l':
			gamepad[RIGHT_BUTTON].is_down = 0;
			break;
		case 'a':
			gamepad[ROLL_L_BUTTON].is_down = 0;
			gamepad[ROLL_L_BUTTON].timer = 0;
			break;
		case 'd':
			gamepad[ROLL_R_BUTTON].is_down = 0;
			gamepad[ROLL_R_BUTTON].timer = 0;
			break;
		case 'w':
			gamepad[BOOST_BUTTON].is_down = 0;
			gamepad[BOOST_BUTTON].timer = 0;
			INCREASE_SPEED = 0;
			break;
		case 's':
			gamepad[BRAKE_BUTTON].is_down = 0;
			gamepad[BRAKE_BUTTON].timer = 0;
			DECREASE_SPEED = 0;
			break;
		case ' ':
			gamepad[FIRE_BUTTON].is_down = 0;
			FIRE_LASERS = 0;
			break;
		case 'p':
			gamepad[PAUSE_BUTTON].is_down = 0;
			gamepad[PAUSE_BUTTON].timer = 0;
			break;
	}
}

void player :: update_rotation()
{
	float degrees;
	float world_x[4] = { 1, 0, 0, 0 };
	degrees = acos(vv_dot(ship->local_x, world_x));
	cout << degrees << endl;
}
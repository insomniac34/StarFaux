//written by Tyler Raborn

#include "PowerUp.h"

static GLfloat GREEN[3] = {0.15, 0.5, 0.1};
static GLfloat DARK_GREEN[3] = {0.0, 0.3, 0.0};
static GLfloat PURPLE[3] = {0.6, 0.1, 0.6};
static GLfloat RED[3] = {1.0, 0.0, 0.0};
static GLfloat GREY[3] = {.45, .43, .43};
static GLfloat DARK_GREY[3] = {.33, .33, .33};
static GLfloat YELLOW[3] = {1.0, 1.0, 0.0};
static GLfloat BLUE[3] = {0.0, 0.0, 0.9};
static GLfloat BROWN[3] = {.35, .30, .25};
static GLfloat LIGHT_BLUE[3] = {0.0, 0.4, 1.0};
static GLfloat WHITE[3] = {1.0, 1.0, 1.0};

static GLfloat DARK_GREY_3D[4] = {.33, .33, .33, 1.0};
static GLfloat GREEN_3D[4] = {0.15, 0.5, 0.1};
static GLfloat DARK_GREEN_3D[4] = {0.0, 0.3, 0.0, 1.0};
static GLfloat PURPLE_3D[4] = {0.6, 0.1, 0.6, 1.0};
static GLfloat RED_3D[4] = {1.0, 0.0, 0.0, 1.0};
static GLfloat GREY_3D[4] = {.45, .43, .43, 1.0};
static GLfloat YELLOW_3D[4] = {1.0, 1.0, 0.0, 1.0};
static GLfloat BLUE_3D[4] = {0.0, 0.0, 0.9, 1.0};
static GLfloat LIGHT_BLUE_3D[4] = {0.0, 0.4, 1.0, 1.0};
static GLfloat BROWN_3D[4] = {.35, .30, .25, 1.0};
static GLfloat WHITE_3D[4] = {1.0, 1.0, 1.0, 1.0};

PowerUp::PowerUp()
{

}

PowerUp::PowerUp(Shape *S, float healthAmt, float boostAmt)
{
	this->targetShape = S;
	this->boostBoost = boostAmt;
	this->healthBoost = healthAmt;

	this->bounceStatus = 0;
}

void PowerUp::update(GLfloat *playerPosition, float *playerHealth, float *playerBoost)
{
	if (this->getStatus() == false)
	{		
		GLfloat *currentPosition = this->targetShape->getPosition();
		if (Vertex(currentPosition).getDistance(playerPosition) < 2.0 && *playerHealth < 1.0) 
		{
			if (*playerHealth > .75)
			{
				*playerHealth = 1.0;
			}
			else
			{	
				*playerHealth += POWERUP_HEALTH_BOOST;
				this->isTaken = true;
			}
		}
		else //movement
		{
			int vAmt = rand() % 15;
			if (vAmt < 10) vAmt = 10;

			this->targetShape->setVS(vAmt); //causes random wobbling

			GLfloat *curObjPos = this->targetShape->getPosition();

			if (this->bounceStatus == 0 || this->bounceStatus == 1)
			{
				this->targetShape->translate(
					                         POWERUP_SHIFT_AMT_X,
					                         POWERUP_SHIFT_AMT_Y,
					                         POWERUP_SHIFT_AMT_Z
					                        );

				this->targetShape->setColor(WHITE);
				this->bounceStatus++;
			}
			else if (this->bounceStatus == 2 || this->bounceStatus == 3)
			{
				this->targetShape->translate(
					                         (-1*POWERUP_SHIFT_AMT_X),
					                         (-1*POWERUP_SHIFT_AMT_Y),
					                         (-1*POWERUP_SHIFT_AMT_Z)
					                        );

				this->targetShape->setColor(LIGHT_BLUE);
				this->bounceStatus++;
			}
			else
			{
				this->targetShape->setColor(PURPLE);
				this->bounceStatus = 0;
			}

			delete[] curObjPos;

			this->targetShape->draw(GL_QUADS);
		}

		delete[] currentPosition;
	}
}

bool PowerUp::getStatus()
{
	return this->isTaken;
}

PowerUp::~PowerUp()
{

}
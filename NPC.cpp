/* 
 *												   Author: Tyler Raborn
 *
 *                                         (C) 2014 Tyler Raborn & Charlie Koch
 *
 * 	This code is freely distributable under the GNU General Public License. See LICENSE.txt for more details.	       					
 */				  

#include "NPC.h"
#include "Projectile.h"
#include "config.h"

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

NPC::NPC()
{

}

NPC::NPC(int iff, int AI, Shape3D *S, Path *P)
{
	this->iff = iff;
	this->AI = AI;
	this->targetShape = S;
	this->targetPath = P;
	this->health = 1.0; //set enemy health
	spawnLock = 0;

	this->currentWaypoint[0] = P->startPoint.x;
	this->currentWaypoint[1] = P->startPoint.y;
	this->currentWaypoint[2] = P->startPoint.z;

	GLfloat *initialPosition = targetShape->getPosition();

	Vertex pos = Vertex(initialPosition);
	currentDirection = vec3(targetPath->startPoint, pos);

	vec3 *tempVec = currentDirection.normalize();
	currentDirection = vec3(tempVec->x, tempVec->y, tempVec->z);

	delete tempVec;
	delete[] initialPosition;

	this->waypointIterator = this->targetPath->waypoints.begin();

	pathComplete = false;

	fireLock = 0;
	fireCounter = 0;

	isDead = 0;
	explosionWidthX = 1.0;
	explosionWidthY = 1.0;
	explosionWidthZ = 1.0;
	colorFlag = 0;
}

Shape3D *NPC::getShape()
{
	return targetShape;
}

Path *NPC::getPath()
{
	return targetPath;
}

void NPC::update(
	             GLfloat inc, 
	             std::deque<Projectile> *projectileList,
	             GLfloat *playerPosition, 
	             int* player_kills, 
	             std::deque<PowerUp*> *powerUpList, 
	             std::list<NPC*> *hostileList, 
	             std::list<NPC*> *friendList,
	             bool notFinalZone
	            ) 
{
	if (this->health > 0)
	{
		fireCounter++; //for calculating burst intervals
		if (fireCounter > 13)
		{
			fireCounter = 0;
		}

		if (pathComplete == false)
		{
			GLfloat *curPos = targetShape->getPosition();
			Vertex NPCposition = Vertex(curPos);

			if (NPCposition.getDistance(currentWaypoint) > 2.0)
			{
				//multiply each positional component by some set fraction of the normalized direction vector and add it to that component of the current position:
				GLfloat dX = (inc * this->currentDirection.x); 
				GLfloat dY = (inc * this->currentDirection.y);
				GLfloat dZ = (inc * this->currentDirection.z);

				this->targetShape->local_translate(dX, dY, dZ); //move the object			
			}
			else if (NPCposition.getDistance(currentWaypoint) == 0) //swap out next waypoint
			{
				this->waypointIterator++; //update waypoint tracker

				if ((waypointIterator->x != targetPath->waypoints.end()->x) && (waypointIterator->y != targetPath->waypoints.end()->y) && (waypointIterator->z != targetPath->waypoints.end()->z))
				{
					this->currentWaypoint[0] = this->waypointIterator->x;
					this->currentWaypoint[1] = this->waypointIterator->y;
					this->currentWaypoint[2] = this->waypointIterator->z;	

					//calculate direction vector & normalize:
					Vertex wpNext = Vertex(currentWaypoint);

					this->currentDirection = vec3(wpNext, NPCposition);
					vec3 *tempDir = this->currentDirection.normalize();
					this->currentDirection = vec3(tempDir->x, tempDir->y, tempDir->z);

					delete tempDir;
				}	
				else 
				{
					this->pathComplete = true; //NPC is now eligible for explicit garbage collection
				}
			}
			else //translate to current waypoint
			{
				//calculate distance to translate:
				GLfloat xInc = this->currentWaypoint[0] - curPos[0];
				GLfloat yInc = this->currentWaypoint[1] - curPos[1];
				GLfloat zInc = this->currentWaypoint[2] - curPos[2];
	 
				this->targetShape->local_translate(xInc, yInc, zInc); //move the object
			}

			GLfloat *updatedPosition = targetShape->getPosition();

			this->targetShape->draw_mesh(GL_TRIANGLES, GL_FLAT);

			//if (fireCounter==2 || fireCounter==5 || fireCounter==7) //original super fast pace
			if ((fireCounter==0 || fireCounter==7) && (playerPosition[2] >= updatedPosition[2])) 
			{
				GLfloat targetDistance = Vertex(updatedPosition).getDistance(playerPosition);

				if (friendList->size() > 0) //check to see if a friendly NPC presents a better target then the player; if so, attack it instead.
				{
					GLfloat *optimalFriendlyPosition;
					GLfloat optimalFriendlyDistance = 999999.0;

					for (std::list<NPC*>::iterator iter = friendList->begin(); iter != friendList->end(); iter++)
					{
						GLfloat *friendlyPosition = (*iter)->getShape()->getPosition();
						GLfloat friendlyDistance = Vertex(friendlyPosition).getDistance(updatedPosition);

						if (friendlyDistance < targetDistance && friendlyDistance < 50.0 && friendlyDistance < optimalFriendlyDistance)
						{
							optimalFriendlyDistance = friendlyDistance;
							optimalFriendlyPosition = friendlyPosition;
						}

						if (optimalFriendlyDistance >= targetDistance) 
						{
							delete[] friendlyPosition;
						}
					}

					if (optimalFriendlyDistance < targetDistance)
					{
						targetDistance = optimalFriendlyDistance;
						playerPosition = optimalFriendlyPosition;
					}
				}

				if (targetDistance < 50.0 && (this->iff == NPC_HOSTILE))
				{
					Shape *burst0;
					burst0 = new Shape(SHAPE_CONE, 4, 4, 1.0, RED);
					burst0->tessellate();
					burst0->scale(.2, 1.6, .2);
					burst0->translate(updatedPosition[0], updatedPosition[1], updatedPosition[2]);
					burst0->revolve(90.0, 0, 0, 1);

					//offset for ship location:
					playerPosition[1]-=2;
					playerPosition[2]-=4;

					//modify accuracy based on AI level...
					playerPosition[0] += (rand() % (this->AI + 1));
					playerPosition[1] += (rand() % (this->AI + 1));
					playerPosition[2] += (rand() % (this->AI + 1));

					vec3 fireVec = vec3(Vertex(playerPosition), Vertex(updatedPosition));
					vec3 *tempFireVec = fireVec.normalize();
					fireVec = vec3(tempFireVec->x, tempFireVec->y, tempFireVec->z);

					projectileList->push_front(Projectile(LASER_SPEED_SUPER_SLOW, &fireVec, burst0, 200.0, 1));

					delete tempFireVec;
				}
				else if (this->iff == NPC_FRIENDLY && (fireCounter == 0 || fireCounter == 7)) //FRIENDLY NPC firing code
				{
					NPC *optimalTarget;
					GLfloat optimalTargetDistance = 999999.0;

					for (std::list<NPC*>::iterator iter = hostileList->begin(); iter != hostileList->end(); iter++)
					{
						GLfloat *targetPosition = (*iter)->getShape()->getPosition();
						GLfloat curDistanceToTarget = Vertex(targetPosition).getDistance(updatedPosition);

						if ((curDistanceToTarget < 50.0) && (curDistanceToTarget < optimalTargetDistance) && (updatedPosition[2] > targetPosition[2]))
						{
							optimalTargetDistance = curDistanceToTarget;
							optimalTarget = (*iter);
						}

						delete[] targetPosition;
					}

					if (optimalTargetDistance < 50.0)
					{
						Shape *friendlyBurst0;
						friendlyBurst0 = new Shape(SHAPE_CONE, 4, 4, 1.0, YELLOW);
						friendlyBurst0->tessellate();
						friendlyBurst0->scale(.2, 1.6, .2);
						friendlyBurst0->translate(updatedPosition[0], updatedPosition[1], updatedPosition[2]);
						friendlyBurst0->revolve(90.0, 0, 0, 1);

						GLfloat *optimalTargetPosition = optimalTarget->getShape()->getPosition();

						//modify accuracy based on AI level...
						optimalTargetPosition[0] += (rand() % (this->AI + 1));
						optimalTargetPosition[1] += (rand() % (this->AI + 1));
						optimalTargetPosition[2] += (rand() % (this->AI + 1));

						vec3 friendlyFireVec = vec3(Vertex(optimalTargetPosition), Vertex(updatedPosition));
						vec3 *tempFriendlyFireVec = friendlyFireVec.normalize();
						friendlyFireVec = vec3(tempFriendlyFireVec->x, tempFriendlyFireVec->y, tempFriendlyFireVec->z);

						//printf("FRIENDLY NPC FIRING BURST; fireCounter = %d\n", fireCounter);

						projectileList->push_front(Projectile(LASER_SPEED_SUPER_SLOW, &friendlyFireVec, friendlyBurst0, GL_FAR_CLIPPING_PLANE, 0));	

						delete[] optimalTargetPosition;
						delete tempFriendlyFireVec;						
					}					
				}
			}
			delete[] updatedPosition;
			delete[] curPos;
		}
		else
		{
			//printf("Path complete!\n");
		}
/*
		for (std::list<NPC*>::iterator iter = friendList->begin(); iter != friendList->end(); iter++)
		{
			if ((*iter) != this)
			{
				GLfloat *npcPos = (*iter)->getShape()->getPosition();
				GLfloat *thisPos = this->getShape()->getPosition();
				GLfloat distanceToTarget = Vertex(npcPos).getDistance(thisPos);

				if (distanceToTarget < 4.0)
				{
					if (npcPos[0] > thisPos[0]) //if npc is ahead of this npc
					{
						//this->adjustBearing(0.08);
					}
					else
					{
						//this->adjustBearing(-0.08);
					}

					if (npcPos[1] > thisPos[1])
					{
						//this->adjustAltitude(0.08);
					}
					else
					{
						//this->adjustAltitude(-0.08);
					}

					if (npcPos[2] > thisPos[2])
					{
						
					}

				}
			}
		}
*/
	}
	else if (this->isDead==0) //WARNING! MEMORY LEAK!! need to delete the previous shape but it crashes.
	{
		*player_kills = (*player_kills + 1);

		if (this->iff == NPC_FRIENDLY)
		{
			explosionWidthX *= 2;
			explosionWidthY *= 2;
			explosionWidthZ *= 2;
		}

		Shape3D *shpPtr = this->targetShape;
		GLfloat *shapePos = this->targetShape->getPosition();

		this->targetShape = new Sphere(1.0, 10, 10);
		this->targetShape->local_translate(shapePos[0], shapePos[1], shapePos[2]);
		this->targetShape->local_scale(explosionWidthX, explosionWidthY, explosionWidthZ);
		this->targetShape->set_color(YELLOW_3D);

		this->targetShape->draw(1);

		this->isDead = 1;

		//powerup drop code:
		int powerUpRand = (rand() % POWERUP_DROP_FREQ_LOW);
		if (notFinalZone == true && powerUpRand == 0)
		{
			Shape *curPowerUpObj;
			curPowerUpObj = new Shape(SHAPE_SPHERE, 15.0, 15.0, 1.0, LIGHT_BLUE);
			curPowerUpObj->tessellate();
			curPowerUpObj->scale(.5, .5, .5);

			curPowerUpObj->translate(
				                     this->targetShape->local_p[0],
				                     this->targetShape->local_p[1],
				                     this->targetShape->local_p[2]
				                    );

			PowerUp *pUp0 = new PowerUp(curPowerUpObj, .25, 0.0);
			powerUpList->push_front(pUp0);
		}
	}
	else //Boom.
	{
		glDisable(GL_LIGHTING);
		if (explosionWidthX > 0)
		{
			if (colorFlag==0)
			{
				this->targetShape->set_color(YELLOW_3D);
				colorFlag++;
			}
			else if (colorFlag==1)
			{
				this->targetShape->set_color(RED_3D);
				colorFlag++;
			}
			else if (colorFlag==2)
			{
				this->targetShape->set_color(WHITE_3D);
				colorFlag = 0;
			}

			explosionWidthX-=0.01;
			explosionWidthY-=0.01;
			explosionWidthZ-=0.01;
			
			this->targetShape->local_scale(explosionWidthX, explosionWidthY, explosionWidthZ);
			this->targetShape->draw(1);
		}
		glEnable(GL_LIGHTING);
	}
}

GLfloat *NPC::getPosition()
{
	//return (this->targetShape->getPosition());
	GLfloat *ret = new GLfloat[3];
	ret[0] = this->targetShape->local_p[0];
	ret[1] = this->targetShape->local_p[1];
	ret[2] = this->targetShape->local_p[2];
	return ret;
}

int NPC::getIFF() //friend or foe?
{
	return this->iff;
}

void NPC::adjustAltitude(float amt)
{
	this->getShape()->local_translate(0.0, amt, 0.0);
	//this->getShape()->draw_mesh(GL_TRIANGLES, GL_FLAT);
}

void NPC::adjustBearing(float amt)
{
	this->getShape()->local_translate(amt, 0.0, 0.0);
	//this->getShape()->draw_mesh(GL_TRIANGLES, GL_FLAT);
}

NPC::~NPC()
{
	printf("\necho @ NPC destructor -> CLEAN EXIT!!\n");
}

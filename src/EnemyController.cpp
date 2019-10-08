#include "EnemyController.h"

EnemyController::EnemyController() {}

//Assigns initial values to the relevant variable
void EnemyController::Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, class EnemySpawner &spawn, int type, int ID)
{
	xPos = x;
	yPos = y;
	width = 64;
	height = 64;
	//The initial position and size of the sprite

	spawn.numberCurrent++;

	SDL_Log("[%s] %i,%i",getTime(), xPos, yPos);

	attackHitBox = new SDL_Rect();
	hitBox = new SDL_Rect();

	srcRect = { 0, 0, 32, 32 };
	dstRect = { xPos, yPos, width, height };

	texture = aTexture;

	attackType = type;

	//Determines which attack range is needed according to which attack type was assigned to them
	if (attackType == 1)
	{
		rangeToUse = meleeRange;
	}
	else
	{
		rangeToUse = rangedRange;
	}

	moving = true;
	attacking = false;
	startAttack = false;
	//Enemies will always initialise in the moving animation	
	
	enemyVelocity.x = 0;
	enemyVelocity.y = 0;
	gravityScale = 1;
	terminalVelocity = 10;
	grounded = false;

	enemyID = ID;
}

void EnemyController::Update(SDL_Window* window, WorldGen &worldGen, class PlayerController &playerController, class EnemySpawner &spawn)
{
	//Get the number of ticks.. we'll use this to calculate what frame number we should be at
	ticks = SDL_GetTicks();
	//The delay between changing frames -- this will change the texture every 200ms.
	changeTimeMS = 200;
	//Calculate frame index -- this is just (time / change) mod frameCount
	frameIndex = (ticks / changeTimeMS) % 4;
		
	minX = xPos + 16;
	maxX = minX + width - 32;
	minY = yPos + 4;
	maxY = minY + height - 6;

	if (facing == 0)
	{
		attackMinX = xPos + width - 20;
		attackMaxX = xPos + width;
	}
	else
	{
		attackMinX = xPos;
		attackMaxX = xPos + 20;
	}	

	attackMaxY = minY + height - 6;
	attackMinY = attackMaxY - 18;

	attackHitBox->x = attackMinX;
	attackHitBox->y = attackMinY;
	attackHitBox->w = attackMaxX - attackMinX;
	attackHitBox->h = attackMaxY - attackMinY;

	hitBox->x = minX;
	hitBox->y = minY;
	hitBox->w = maxX - minX;
	hitBox->h = maxY - minY;	
		
	worldCollisionType = DetectCollisionWorld(worldGen);
	enemyCollisionType = DetectCollisionEnemy(spawn);
	playerCollisionType = DetectCollisionPlayer(playerController);

	if (worldCollisionType == 0)
	{
		if (facing == 0)
		{
			facing = 2;
		}
		else
		{
			facing = 0;
		}
	}

	if (worldCollisionType >= 2 || enemyCollisionType >= 2 || playerCollisionType >= 2)
	{
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	/*SDL_Log("Collision Type : %i",worldCollisionType);*/

	if (grounded == false)
	{
		//Enemy Moving Down	
		enemyVelocity.y += gravityScale;	
	}
	else
	{
		if (enemyVelocity.y != 0)
		{
			SDL_Log("[%s] Enemy %i Landed", getTime(), enemyID);
			enemyVelocity.y = 0;
		}			

		if (worldCollisionType == 1 || worldCollisionType == 3 || enemyCollisionType == 1 || enemyCollisionType == 3 || playerCollisionType == 1 || playerCollisionType == 3)
		{
			if (facing == 0)
			{
				facing = 2;
			}
			else
			{
				facing = 0;
			}
		}		
	}

	if (facing == 0)
	{
		enemyVelocity.x = speed;
	}
	else
	{
		enemyVelocity.x = -speed;
	}

	if (worldCollisionType == 2 || worldCollisionType == 3 || enemyCollisionType == 2 || enemyCollisionType == 3 || playerCollisionType == 2 || playerCollisionType == 3)
	{
		enemyVelocity.y = 0;
		yPos += 2;
	}
	
	if (InRange(playerController) == true)
	{
		enemyVelocity.x = 0;
		//Player Attacking
		if (attacking == false)
		{
			startAttack = true;
			attackConnected = false;
			attacksmade++;
		}
		if (attacking == true)
		{
			startAttack = false;
		}
	}
	else
	{
		if (facing == 0)
		{
			enemyVelocity.x = speed;
		}
		else
		{
			enemyVelocity.x = -speed;
		}
	}

	xPos += enemyVelocity.x;
	yPos += enemyVelocity.y;

	//Changes the x value of the Enemy sprite's source location
	calcX = (frameWidthPixels * (frameIndex));
	calcY = (frameHeightPixels * facing);
		
	if (attacking == true || startAttack == true)
	{
		//Only called at the start of a enemy's attack
		if (startAttack == true)
		{
			//Initiates the Enemy Attack
			attackFramesIndex = 0;
			startAttack = false;
			SDL_Log("[%s] Enemy %i: Attack Started", getTime(), enemyID);
			attacking = true;
		}
		//Cycles through the Enemy's attack frames
		else if (attackFramesIndex < attackFramesLimit - 1)
		{
			//The delay between changing frames -- this will change the texture every 200ms.
			changeTimeMS = 75;
			//Calculate frame index -- this is just (time / change) mod frameCount
			attackFramesIndex = (ticks / changeTimeMS) % attackFramesLimit;

			Attack(playerController);
		}
		//Resets the enemy's attack so that they can attack again
		else
		{
			attackFramesIndex = 0;
			attacking = false;
			SDL_Log("[%s] Enemy %i: Attack Ended", getTime(), enemyID);
		}

		calcX = (frameWidthPixels * (attackFramesIndex + movementFramesLimit));
		calcY = (frameHeightPixels * facing);
		moving = false;
	}
	else
	{
		//If the enemy is not attacking then they can begin their path once more
		moving = true;
	}		
		
	dstRect = { xPos, yPos, width, height };

	srcRect = { calcX, calcY, frameWidthPixels, frameHeightPixels };
	//Reallocates the Enemy sprite's source and destination
}

void EnemyController::Render(SDL_Renderer* aRenderer)
{
	//SDL_SetRenderDrawColor(aRenderer, 0, 0, 255, 255);

	//SDL_RenderDrawRect(aRenderer, attackHitBox);

	//SDL_SetRenderDrawColor(aRenderer, 255, 0, 255, 255);

	//SDL_RenderDrawRect(aRenderer, hitBox);

	SDL_RenderCopy(aRenderer, texture, &srcRect, &dstRect);	
}

void EnemyController::Health(int damageTaken)
{
	currentHealth -= damageTaken;
	
	SDL_Log("[%s] Enemy %i: Health = %i, Max Health = %i", getTime(), enemyID, currentHealth, maxHealth);
}

//No Collision = 0, Horizontal Collision = 1, Vertical Collision = 2
int EnemyController::DetectCollisionEnemy(class EnemySpawner &spawn)
{
	bool yCollision = false;
	bool xCollision = false;

	for (auto& element : spawn.enemyList)
	{		
		if (element != this)
		{
			if (((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y <= element->maxY) && (maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x <= element->maxX)))
			{
				SDL_Log("[%s] Enemy %i collided with a Enemy %i", getTime(), enemyID, element->enemyID);
				if (!yCollision && ((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y < element->minY)
					|| (maxY + enemyVelocity.y <= element->maxY && minY + enemyVelocity.y > element->maxY)))
				{
					/*SDL_Log("Vertical Collision Detection");*/
					yPos += (element->minY - maxY) - 2;
					minY = yPos + 4;
					maxY = minY + 56;
					yCollision = true;					
				}
			}

			if (((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y <= element->maxY) && (maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x <= element->maxX)))
			{
				if (!xCollision && ((maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x < element->minX)
					|| (minX + enemyVelocity.x <= element->maxX && maxX + enemyVelocity.x > element->maxX)))
				{
				/*	SDL_Log("Horizontal Collision Detected");*/
					xCollision = true;
				}
			}
		}		
	}
	
	int collisionMode = 0;
	if (yCollision) collisionMode += 2;
	if (xCollision) collisionMode += 1;

	return collisionMode;
	
}

//No Collision = 0, Horizontal Collision = 1, Vertical Collision = 2
int EnemyController::DetectCollisionPlayer(class PlayerController &playerController)
{
	bool yCollision = false;
	bool xCollision = false;

	
	if (((maxY + enemyVelocity.y >= playerController.minY && minY + enemyVelocity.y <= playerController.maxY) && (maxX + enemyVelocity.x >= playerController.minX && minX + enemyVelocity.x <= playerController.maxX)))
	{
		if (!yCollision && ((maxY + enemyVelocity.y >= playerController.minY && minY + enemyVelocity.y < playerController.minY)
			|| (maxY + enemyVelocity.y <= playerController.maxY && minY + enemyVelocity.y > playerController.maxY)))
		{
	/*		SDL_Log("Vertical Collision Detection");*/
			yPos += (playerController.minY - maxY) - 2;
			minY = yPos + 4;
			maxY = minY + 58;
			yCollision = true;
			SDL_Log("[%s] Enemy %i collided with a player", getTime(), enemyID);
		}
	}

	if (((maxY + enemyVelocity.y >= playerController.minY && minY + enemyVelocity.y <= playerController.maxY) && (maxX + enemyVelocity.x >= playerController.minX && minX + enemyVelocity.x <= playerController.maxX)))
	{
		if (!xCollision && ((maxX + enemyVelocity.x >= playerController.minX && minX + enemyVelocity.x < playerController.minX)
			|| (minX + enemyVelocity.x <= playerController.maxX && maxX + enemyVelocity.x > playerController.maxX)))
		{
	/*		SDL_Log("Horizontal Collision Detected");*/
			xCollision = true;
			SDL_Log("[%s] Enemy %i collided with a player", getTime(), enemyID);
		}
	}
	

	int collisionMode = 0;
	if (yCollision) collisionMode += 2;
	if (xCollision) collisionMode += 1;

	return collisionMode;
}

int EnemyController::DetectCollisionWorld(WorldGen &worldGen)
{
	bool yCollision = false, xCollision = false;

	//Detects enemy collision with environment
	for (auto& element : worldGen.borderList)
	{
		if (((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y <= element->maxY) && (maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x <= element->maxX)))
		{
			if (!yCollision && ((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y < element->minY) 
				|| (maxY + enemyVelocity.y <= element->maxY && minY + enemyVelocity.y > element->maxY)))
			{
				yPos += (element->minY - maxY) - 2;
				minY = yPos + 4;
				maxY = minY + 58;
				yCollision = true;
			}
		}

		if (((maxY + enemyVelocity.y >= element->minY && minY + enemyVelocity.y <= element->maxY) && (maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x <= element->maxX)))
		{
			if (!xCollision && ((maxX + enemyVelocity.x >= element->minX && minX + enemyVelocity.x < element->minX)
				|| (minX + enemyVelocity.x <= element->maxX && maxX + enemyVelocity.x > element->maxX)))
			{	
				SDL_Log("[%s] Enemy %i collided with a wall", getTime(), enemyID);
				xCollision = true;
			}
		}		
	}

	int collisionMode = 0;
	if (yCollision) collisionMode += 2;
	if (xCollision) collisionMode += 1;

	return collisionMode;
}

void EnemyController::Attack(class PlayerController &playerController)
{
	//Used for Attacking Collisions
	if ((((attackMaxY + enemyVelocity.y >= playerController.minY && attackMinY + enemyVelocity.y <= playerController.maxY) && (attackMaxX + enemyVelocity.x >= playerController.minX && attackMinX + enemyVelocity.x <= playerController.maxX))))
	{
		if (((attackMaxX + enemyVelocity.x >= playerController.minX && attackMinX + enemyVelocity.x < playerController.minX) || (attackMinX + enemyVelocity.x <= playerController.maxX && attackMaxX + enemyVelocity.x > playerController.maxX)))
		{
			if (facing == 0 && (attackMaxX + enemyVelocity.x >= playerController.minX && attackMinX + enemyVelocity.x < playerController.minX) && attackConnected == false && attacking == true)
			{
				attackConnected = true;
				SDL_Log("[%s] Attack From Enemy %i Connected Right With Player", getTime(), enemyID);
				playerController.HealthLost(damage);
			}
			if (facing == 2 && (attackMinX + enemyVelocity.x <= playerController.maxX && attackMaxX + enemyVelocity.x > playerController.maxX) && attackConnected == false && attacking == true)
			{
				attackConnected = true;
				SDL_Log("[%s] Attack From Enemy %i Connected Left With Player", getTime(), enemyID);
				playerController.HealthLost(damage);
			}
		}
	}
}

int EnemyController::GetxPos()
{
	return xPos;
}

int EnemyController::GetyPos()
{
	return yPos;
}

bool EnemyController::InRange(class PlayerController &playerController)
{
	//Used for Attacking Collisions
	if ((((attackMaxY + enemyVelocity.y >= playerController.minY && attackMinY + enemyVelocity.y <= playerController.maxY) && ((abs(playerController.maxX - (minX + enemyVelocity.x)) < rangeToUse) || abs(playerController.minX - (maxX + enemyVelocity.x)) < rangeToUse))))
	{
		SDL_Log("[%s] Enemy %i within range of Player", getTime(), enemyID);
		return true;
	}
	else
	{
		return false;
	}
}

EnemyController::~EnemyController() {}


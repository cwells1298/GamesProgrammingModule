#include"PlayerController.h"

PlayerController::PlayerController() {}

//Assigns initial values to the relevant variable
void PlayerController::Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y)
{
	xPos = x;
	yPos = y;
	width = 64;
	height = 64;
	//The initial position and size of the sprite

	playerVelocity.x = 0;
	playerVelocity.y = 0;
	gravityScale = 2;
	terminalVelocity = 20;
	jumpForce = -30;
	grounded = false;

	attackHitBox = new SDL_Rect();
	hitBox = new SDL_Rect();

	srcRect = { 0, 0, 32, 32 };
	dstRect = { xPos, yPos, width, height };

	texture = aTexture;

	//Load in key bindings from json file
	keyboardControls.LoadKeys("../content/keyboardControls1.json");
}

void PlayerController::Input(SDL_Event event, SDL_Window* window, SDL_Joystick* joystick1)
{
	SDL_PumpEvents();

	moving = false;	

	controllerVelocity.x = (SDL_JoystickGetAxis(joystick1, 0) * 2/ 3276.70f);
	controllerVelocity.y = (SDL_JoystickGetAxis(joystick1, 1) * 2/ 3276.70f);
	//SDL_Log("[%s] Joystick X: %i", getTime(), controllerVelocity.x);

	if (event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_LEFT)
	{
		dPadLeft = !dPadLeft;
	}
	if (event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_RIGHT)
	{
		dPadRight = !dPadRight;
	}
	//Determine if player is attacking via controller input
	if (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == controllerAttack)
	{
		controllerAttackActive = true;
	}
	else if (event.type == SDL_JOYBUTTONUP && event.jbutton.button == controllerAttack)
	{
		controllerAttackActive = false;
	}

	//Determine if player is jumping via controller input
	if (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == controllerJump)
	{
		controllerJumpActive = true;
	}
	else if (event.type == SDL_JOYBUTTONUP && event.jbutton.button == controllerJump)
	{
		controllerJumpActive = false;
	}

	//Moves the player sprite using WASD according to player speed
	if ((dPadRight || state[keyboardControls.moveRight]) && !state[keyboardControls.moveLeft] )
	{
		//Player Moving Right
		playerVelocity.x = speed;
		facing = 0;
		moving = true;
	}
	else if ((dPadLeft || state[keyboardControls.moveLeft]) && !state[keyboardControls.moveRight])
	{
		//Player Moving Left
		playerVelocity.x = -speed;
		facing = 2;
		moving = true;
	}
	else
	{
		playerVelocity.x = 0;
	}

	if (controllerVelocity.x > 5)
	{
		playerVelocity.x = (controllerVelocity.x - 5) / 3;
		facing = 0;
		moving = true;
	}
	if (controllerVelocity.x < -5)
	{
		playerVelocity.x = (controllerVelocity.x + 5) / 3;
		facing = 2;
		moving = true;
	}

	if (((state[keyboardControls.jump] || controllerJumpActive) == true && grounded == true))
	{
		//Player Jumping
		playerVelocity.y = jumpForce;
		jumpAudioTrigger = true;
		grounded = false;
		SDL_Log("[%s] Player Jumping", getTime());
	}		

	if (state[keyboardControls.attack] || controllerAttackActive == true)
	{
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
}

void PlayerController::Update(SDL_Window* window, WorldGen &worldGen, class EnemySpawner &spawn)
{
	//Get the number of ticks.. we'll use this to calculate what frame number we should be at
	ticks = SDL_GetTicks();
	//The delay between changing frames -- this will change the texture every 200ms.
	changeTimeMS = 200;
	//Calculate frame index -- this is just (time / change) mod frameCount
	movementFramesIndex = (ticks / changeTimeMS) % movementFramesLimit;		
		
	if (moving == true)
	{
		//Changes the x value of the player sprite's source location
		calcX = (frameWidthPixels * movementFramesIndex);
		calcY = (frameHeightPixels * facing);

		if (facing != lastFacing)
		{
			if (facing == 0)
			{
				SDL_Log("[%s] Player Direction Changed: Right", getTime());
			}	
			else
			{
				SDL_Log("[%s] Player Direction Changed: Left", getTime());
			}
		
			lastFacing = facing;
		}
	}
	else
	{
		//Set to idle sprite
		calcX = 32;
	}	

	minX = xPos + 16;
	maxX = xPos + width - 20;
	minY = yPos + 4;
	maxY = minY + height - 6;

	if (facing == 0)
	{
		attackMinX = xPos + width - 20;	
	}
	else
	{
		attackMinX = xPos + 4;	
	}

	attackMaxX = attackMinX + 12;
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

	enemyCollisionType = DetectCollisionEnemies(spawn);
	worldCollisionType = DetectCollisionWorld(worldGen,window);
	if ((worldCollisionType >= 2 || enemyCollisionType >= 2) && !hitCeiling)
	{
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	/*SDL_Log("World Collision Type : %i", worldCollisionType);*/

	if (grounded == false)
	{
		//player Moving Down	
		playerVelocity.y += gravityScale;
	}
	else
	{
		if (playerVelocity.y != 0)
		{
			SDL_Log("[%s] Player Landed", getTime());
			playerVelocity.y = 0;
		}

		if (worldCollisionType == 1 || worldCollisionType == 3 || enemyCollisionType == 1 || enemyCollisionType == 3)
		{
			playerVelocity.x = 0;
		}
	}	

	if (worldCollisionType == 2 || worldCollisionType == 3 || enemyCollisionType == 2 || enemyCollisionType == 3)
	{
		if (hitCeiling == true)
		{
			yPos -= 2;
		}
		else
		{			
			yPos += 2;
		}
		playerVelocity.y = 0;
	}
	
	xPos += playerVelocity.x;
	yPos += playerVelocity.y;

	if ((maxX > SDL_GetWindowSurface(window)->w - 64))
	{
		xPos = SDL_GetWindowSurface(window)->w - 128;
		SDL_Log("[%s] Player collided with a wall", getTime());
	}
	else if(minX < 64)
	{
		xPos = 64;
	}

	if (attacking == true || startAttack == true)
	{				
		//Only called at the start of a player's attack
		if (startAttack == true)
		{
			//Initiates the Player Attack
			attackFramesIndex = 0;
			attacking = true;
			startAttack = false;
			SDL_Log("[%s] Attack Started, Number: %i", getTime(), attacksmade);		
			SDL_Log("[%s] Attack Frame : %i", getTime(), attackFramesIndex);					
		}
		//Cycles through the Player's attack frames
		else if(attackFramesIndex < attackFramesLimit - 1)
		{		
			//Calculate frame index -- this is just (time / change) mod frameCount
			//attackFramesIndex = (SDL_GetTicks() / updateDelay) % attackFramesLimit;

			if (ticks > attackLast + attackDelay)
			{
				attackFramesIndex++;
				attackLast = ticks;
			}

			SDL_Log("[%s] Attack Frame : %i", getTime(), attackFramesIndex);
		}
		//Resets the players's attack so that they can attack again
		else
		{
			attackFramesIndex = 0;
			attacking = false;
			SDL_Log("[%s] Attack Ended, Number: %i", getTime(), attacksmade);
		}
		
		calcX = (frameWidthPixels * (attackFramesIndex + movementFramesLimit));
		calcY = (frameHeightPixels * facing);
	}	
	
	//Applies a red filter to the player when they get damaged
	if (damageReceding == true)
	{
		SDL_SetTextureColorMod(texture, colourR + (10 * colourIndex), 0, 0);
		colourIndex++;

		if (colourIndex >= colourIndexLimit)
		{
			damageReceding = false;
			SDL_Log("[%s] Player Damage Receded", getTime());
		}
		
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, 255);
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		colourIndex = 0;
	}

	dstRect = { xPos, yPos, width, height };
	srcRect = { calcX, calcY, frameWidthPixels, frameHeightPixels };
	//Reallocates the player sprite's source and destination
}

void PlayerController::Render(SDL_Renderer* aRenderer)
{
	//SDL_SetRenderDrawColor(aRenderer, 0, 0, 255, 255);*
	//SDL_RenderDrawRect(aRenderer, attackHitBox);

	//SDL_SetRenderDrawColor(aRenderer, 255, 0, 255, 255);

	//SDL_RenderDrawRect(aRenderer, hitBox);

	//Renders the player sprite to screen
	SDL_RenderCopyEx(aRenderer, texture, &srcRect, &dstRect, angle, center, flip);
}

int PlayerController::DetectCollisionEnemies(class EnemySpawner &spawn)
{
	bool yCollision = false;
	bool xCollision = false;

	for (auto& element : spawn.enemyList)
	{				
		
		if (((maxY + playerVelocity.y >= element->minY && minY + playerVelocity.y <= element->maxY) && (maxX + playerVelocity.x >= element->minX && minX + playerVelocity.x <= element->maxX)))
		{
			//Used for Vertical Collisions
			if (!yCollision && ((maxY + playerVelocity.y >= element->minY && minY + playerVelocity.y < element->minY) || (maxY + playerVelocity.y <= element->maxY && minY + playerVelocity.y > element->maxY)))
			{
			/*	SDL_Log("Vertical Collision Detection");*/
				yPos += (element->minY - maxY) - 2;
				minY = yPos + 4;
				maxY = minY + 58;
				yCollision = true;
				SDL_Log("[%s] Player collided vertically with Enemy %i", getTime(), element->enemyID);
			}

			//Used for Horizontal Collisions
			if (!xCollision && ((maxX + playerVelocity.x >= element->minX && minX + playerVelocity.x < element->minX) || (minX + playerVelocity.x <= element->maxX && maxX + playerVelocity.x > element->maxX)))
			{
				/*SDL_Log("Horizontal Collision Detected");*/
				xCollision = true;
				SDL_Log("[%s] Player collided horizontally with Enemy %i", getTime(), element->enemyID);
			}				
		}		

		//Used for Attacking Collisions
		if ((((attackMaxY + playerVelocity.y >= element->minY && attackMinY + playerVelocity.y <= element->maxY) && (attackMaxX + playerVelocity.x >= element->minX && attackMinX + playerVelocity.x <= element->maxX))))
		{
			if (((attackMaxX + playerVelocity.x >= element->minX && attackMinX + playerVelocity.x < element->minX) || (attackMinX + playerVelocity.x <= element->maxX && attackMaxX + playerVelocity.x > element->maxX)))
			{
				if (facing == 0 && (attackMaxX + playerVelocity.x >= element->minX && attackMinX + playerVelocity.x < element->minX) && attackConnected == false && attacking == true)
				{
					attackConnected = true;
					SDL_Log("[%s] Attack Connected Right With Enemy %i", getTime(), element->enemyID);
					element->Health(damage);
					enemyHurtAudioTrigger = true;
				}
				if (facing == 2 && (attackMinX + playerVelocity.x <= element->maxX && attackMaxX + playerVelocity.x > element->maxX) && attackConnected == false && attacking == true)
				{
					attackConnected = true;
					SDL_Log("[%s] Attack Connected Left With Enemy %i", getTime(), element->enemyID);
					element->Health(damage);
					enemyHurtAudioTrigger = true;
				}
			}
		}			
	}


	int collisionMode = 0;
	if (yCollision) collisionMode += 2;
	if (xCollision) collisionMode += 1;

	return collisionMode;
}

int PlayerController::DetectCollisionWorld(WorldGen &worldGen, SDL_Window* window)
{
	bool yCollision = false;
	bool xCollision = false;
	hitCeiling = false;

	//Detects enemy collision with environment
	for (auto& element : worldGen.borderList)
	{
		if (((maxY + playerVelocity.y >= element->minY && minY + playerVelocity.y <= element->maxY) && (maxX + playerVelocity.x >= element->minX && minX + playerVelocity.x <= element->maxX)))
		{
			if (!yCollision && (maxY + playerVelocity.y >= element->minY && minY + playerVelocity.y < element->minY))
			{
				/*SDL_Log("Vertical Collision Detection");*/
				yPos += (element->minY - maxY) - 2;
				minY = yPos + 4;
				maxY = minY + 56;
				yCollision = true;
			}

			if (!yCollision && (maxY + playerVelocity.y > element->maxY && minY + playerVelocity.y < element->maxY))
			{
				yPos += (element->maxY - minY) + 2;
				playerVelocity.y = 0;
				minY = yPos + 4;
				maxY = minY + 56;
				yCollision = true;
				hitCeiling = true;
			}
		}

		if (((maxY + playerVelocity.y >= element->minY && minY + playerVelocity.y <= element->maxY) && (maxX + playerVelocity.x >= element->minX && minX + playerVelocity.x <= element->maxX)))
		{
			if (!xCollision && ((maxX + playerVelocity.x >= element->minX && minX + playerVelocity.x < element->minX) || (minX + playerVelocity.x <= element->maxX && maxX + playerVelocity.x > element->maxX)))
			{
				xCollision = true;
				SDL_Log("[%s] Player collided with a wall", getTime());
			}
		}
	}

	int collisionMode = 0;
	if (yCollision) collisionMode += 2;
	if (xCollision) collisionMode += 1;

	return collisionMode;
}

void PlayerController::HealthLost(int damageTaken)
{		
	if (damageReceding == false)
	{
		SDL_SetTextureColorMod(texture, colourR, 0, 0);
		damageReceding = true;
		currentHealth -= damageTaken;
		playerHurtAudioTrigger = true;
		SDL_Log("[%s] Player: Health = %i, Max Health = %i", getTime(), currentHealth, maxHealth);
	}			

	if (currentHealth <= 0)
	{
		dead = true;
	}
}

void PlayerController::HealthGained(int healAmount)
{
	currentHealth += healAmount;

	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}

	SDL_Log("[%s] Player: Health = %i, Max Health = %i", getTime(), currentHealth, maxHealth);
}

void PlayerController::ScoreGained(int scoreAmount)
{
	playerScore += scoreAmount;
	
	SDL_Log("[%s] Player: Score = %i", getTime(), playerScore);
}

PlayerController::~PlayerController() {}

#pragma once
#include <iostream> // functions for printing things out
#include "SDL.h"	// functions for SDL
#include "EnemySpawner.h"
#include "Vector2D.h"
#include "WorldGen.h"
#include "PlayerController.h"

#include "Logging.h"

class EnemyController
{
private:
	//Used for the positioning and sizing of the sprite
	int xPos;
	int yPos;
	int width;
	int height;

	//Determines Enemy Characteristics;
	int speed = 3;
	int maxHealth = 20;
	int damage = 10;

	//Determines Enemy Direction (for attacking)
	//0=Right,2=Left
	int facing = 0;
	//Used for Logging purposes
	int lastFacing = 0;

	//Used to determine which sprite on the spritesheet needs using
	int calcX = 0;
	int calcY = 0;

	//Used for the source position of the sprite
	int frameWidthPixels = 32;
	int frameHeightPixels = 32;

	//Used to locate and display the required sprite
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	//Used to determine which animation frame needs to be used
	int framecount = 0;
	unsigned int ticks;
	unsigned int changeTimeMS;
	unsigned int frameIndex;
	unsigned int movementFramesIndex = 0;
	unsigned int movementFramesLimit = 4;
	unsigned int attackFramesIndex = 0;
	unsigned int attackFramesLimit = 6;
	int attackDelay = 125;
	int attackLast = 0;
	int attacksmade = 0;

	int updateDelay = 75;	

	//Used to determine which attack type this enemy uses, 1=melee,2=ranged
	int attackType;

	//Determines how close an enemy needs to be to attack
	int meleeRange = 10;
	int rangedRange = 200;
	int rangeToUse;

	//Determine if Enemy is moving/attacking
	bool moving = false;
	bool attacking = false;
	bool startAttack = false;
	bool attackConnected = false;

	//Deals with enemy and physics
	
	int gravityScale;
	int terminalVelocity;
	bool grounded;

	//0=none,1=horizontal,2=vertical,3=both
	int worldCollisionType = 0;
	int enemyCollisionType = 0;
	int playerCollisionType = 0;
	int arrowCollisionType = 0;
		
	//Used to indicate hit boxes present
	SDL_Rect* attackHitBox;
	SDL_Rect* hitBox;

	//Used to extend collision box when attacking
	int attackMinX = 0;
	int attackMaxX = 0;
	int attackMinY = 0;
	int attackMaxY = 0;

public:
	EnemyController();

	void Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, class EnemySpawner &spawn, int type, int ID);
	
	void Update(SDL_Window* window, WorldGen &worldGen, class PlayerController &playerController, class EnemySpawner &spawn);

	void Render(SDL_Renderer* aRenderer);

	void Health(int damageTaken);

	int DetectCollisionEnemy(class EnemySpawner &spawn);

	int DetectCollisionPlayer(class PlayerController &playerController);

	int DetectCollisionWorld(WorldGen &worldGen);

	void Attack(class PlayerController &playerController);

	int GetxPos();

	int GetyPos();

	bool InRange(class PlayerController &playerController);
	
	~EnemyController();

	//Used for collision detection
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;

	Vector2D enemyVelocity;

	int currentHealth = 20;

	int enemyID = 0;
};

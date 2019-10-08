#pragma once
#include <iostream> // functions for printing things out
#include "SDL.h"	// functions for SDL
#include "EnemySpawner.h"
#include "EnemyController.h"
#include "Vector2D.h"
#include "WorldGen.h"
#include "Logging.h"
#include "Controls.h"

class PlayerController
{
private:
	//Used for the positioning and sizing of the sprite
	int xPos;
	int yPos;
	int width;
	int height;

	//Determines Player Characteristics;
	int speed = 5;	
	int damage = 10;

	//Determines Player Direction (for attacking)
	//0=Right,1=Left,2=Up,3=Down
	int facing = 0;
	//Used for Logging purposes
	int lastFacing = 0;

	//Used to determine which sprite on the spritesheet needs using
	int calcX = 0;
	int calcY = 0;

	//Determine if Player is moving/attacking
	bool moving = false;
	bool attacking= false;
	bool startAttack = false;
	bool attackConnected = false;

	//Used for the source position of the sprite
	int frameWidthPixels = 32;
	int frameHeightPixels = 32;

	//Used to locate and display the required sprite
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	//Used to determine values for the extra features required by RenderCopyEx
	double angle = 0;
	const SDL_Point* center = NULL;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Used to determine keyboard input
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//Used to determine which animation frame needs to be used
	int framecount = 0;
	unsigned int ticks;
	unsigned int changeTimeMS;
	unsigned int movementFramesIndex = 0;
	unsigned int movementFramesLimit = 4;
	unsigned int attackFramesIndex = 0;
	unsigned int attackFramesLimit = 6;

	int attackDelay = 125;
	int attackLast = 0;
		
	int attacksmade = 0;

	//Deals with player movement and physics
	Vector2D playerVelocity;
	Vector2D controllerVelocity;
	bool dPadRight = false;
	bool dPadLeft = false;
	bool controllerAttackActive = false;
	bool controllerJumpActive = false;
	int gravityScale;
	int jumpForce;
	int terminalVelocity;
	bool grounded;
	bool hitCeiling;

	bool damageTaken = false;
	bool damageReceding = false;
	int colourIndex = 0;
	int colourIndexLimit = 20;
	int colourR = 40;

	//0=none,1=horizontal,2=vertical,3=both
	int worldCollisionType = 0;
	int enemyCollisionType = 0;
	int pickUpCollisionType = 0;
	int arrowCollisionType = 0;

	//Used to allow for control config
	SDL_Scancode moveLeft = SDL_SCANCODE_A;
	SDL_Scancode moveRight = SDL_SCANCODE_D;
	SDL_Scancode jump = SDL_SCANCODE_W;
	SDL_Scancode attack = SDL_SCANCODE_SPACE;
	int controllerJump = 0;
	int controllerAttack = 2;
	
	//Used to extend collision box when attacking
	int attackMinX = 0;
	int attackMaxX = 0;
	int attackMinY = 0;
	int attackMaxY = 0;

	//Used to indicate hit boxes present
	SDL_Rect* attackHitBox;
	SDL_Rect* hitBox;

	KeyboardControls keyboardControls;

public:
	PlayerController();

	void Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y);

	void Input(SDL_Event event, SDL_Window* window, SDL_Joystick* joystick1);

	void Update(SDL_Window* window, WorldGen &worldGen, class EnemySpawner &spawn);

	void Render(SDL_Renderer* aRenderer);

	int DetectCollisionEnemies(class EnemySpawner &spawn);
	
	int DetectCollisionWorld(WorldGen &worldGen, SDL_Window* window);

	void HealthLost(int damageTaken);

	void HealthGained(int healAmount);
	
	void ScoreGained(int scoreAmount);

	//Used for collision detection
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;

	int playerScore = 0;
	int maxHealth = 100;
	int currentHealth = 100;

	bool dead = false;	
	bool jumpAudioTrigger = false;
	bool enemyHurtAudioTrigger = false;
	bool playerHurtAudioTrigger = false;
	~PlayerController();
};

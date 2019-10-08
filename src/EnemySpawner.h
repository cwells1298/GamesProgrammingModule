#pragma once
#include <iostream> // functions for printing things out
#include <vector>   // functions for using vectors
#include "SDL.h"	// functions for SDL
#include "WorldGen.h"
#include "PickupManager.h"
#include "PlayerController.h"
#include "ParticleManager.h"
#include "Logging.h"

using namespace std;

class EnemySpawner
{
private:
	//Used to locate and display the required sprite
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	SDL_Renderer* aRenderer;

	//Maximum possible number of enemies present at any time
	int maxNumber;

	int enemiesSpawned;

	//Determines where all enemies of this spawner will be spawned
	int xPos = 0;
	int yPos = 0;

	int spawnDelay = 8000;
	int spawnTicks = 0;
	int spawnLast = 0;

public:
	EnemySpawner();

	void Init(SDL_Window* window, SDL_Renderer* aRenderer, SDL_Texture* aTexture, int numberTotal, int eX, int eY);

	void AddEnemy(SDL_Window* window, int xMod, int yMod);

	void Update(SDL_Window* window, WorldGen &worldGen, class PlayerController &playerController, class PickupManager &pickupManager, class ParticleManager &particleManager);

	void Render();

	~EnemySpawner();

	vector<class EnemyController*> enemyList;

	//Number of current enemies 
	int numberCurrent;
};

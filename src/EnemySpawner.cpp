#include "EnemySpawner.h"
#include "EnemyController.h"

EnemySpawner::EnemySpawner() {}

void EnemySpawner::Init(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* aTexture, int numberTotal, int eX, int eY)
{	
	maxNumber = numberTotal;

	texture = aTexture;

	aRenderer = renderer;	

	xPos = eX;
	yPos = eY;

	AddEnemy(window, 1, 1);
	AddEnemy(window, 1, 3);

	spawnLast = SDL_GetTicks();
	SDL_Log("[%s] NumEnemies: %i", getTime(), numberCurrent);
}

void EnemySpawner::AddEnemy(SDL_Window* window, int xMod, int yMod)
{	
	//Determines which attack type the new EnemyController will use
	int type  =1;	
	/*type = 1 + rand() % 2;*/

	enemiesSpawned++;
		
	//Creates a new EnemyController from the generated values and places it with its vector container (enemyList)
	EnemyController* tempEnemy = new EnemyController();
	tempEnemy->Init(aRenderer, texture, xPos * xMod, yPos * yMod, *this, type, enemiesSpawned);	
	this->enemyList.push_back(tempEnemy);

	SDL_Log("[%s] EnemyNo. : %i| X: %i| Y: %i| Attack Type: %i", getTime(), enemiesSpawned, xPos, yPos, type);
}

void EnemySpawner::Update(SDL_Window* window, WorldGen &worldGen, class PlayerController &playerController, class PickupManager &pickupManager, class ParticleManager &particleManager)
{
	//Used to calculate whether it is time for a new enemy to spawn
	spawnTicks = SDL_GetTicks();

	if (spawnTicks > spawnLast + spawnDelay)
	{
		//Adds a new enemy if the limit hasnt been reached
		if (enemiesSpawned < maxNumber * 2)
		{
			AddEnemy(window, 1,1);
			AddEnemy(window, 1,3);
			SDL_Log("[%s] NumEnemies: %i", getTime(), numberCurrent);
			spawnLast = spawnTicks;
		}
	}
	

	int i = 0;
	//Loops through enemyList and updates all EnemyControllers within
	for (auto& element : enemyList)
	{
		element->Update(window,worldGen,playerController,*this);
				
		if (element->currentHealth <= 0)
		{
			particleManager.AddParticles(element->GetxPos() + 32, element->GetyPos() + 32);
			pickupManager.AddPickups(element->GetxPos() + 32, element->GetyPos() + 32);
			enemyList.erase(enemyList.begin() + i);
			SDL_Log("[%s] Enemy Killed... ID: %i", getTime(), element->enemyID);
			enemiesSpawned--;
			break;
		}
		i++;
	}
		
}

void EnemySpawner::Render()
{
	//Loops through enemyList and renders the sprites for all EnemyControllers within
	for (auto& element : enemyList)
	{
		element->Render(aRenderer);
	}
}

EnemySpawner::~EnemySpawner()
{
	//Deletes all subobjects (EnemyControllers) before deleting this object
	for (auto& element : enemyList)
	{
		delete element;
	}
}
#include "PickupManager.h"

PickupManager::PickupManager() {}

void PickupManager::Init(SDL_Texture* aTexture, SDL_Renderer* aRenderer)
{
	texture = aTexture;
	renderer = aRenderer;
}

void PickupManager::AddPickups(int xPos, int yPos)
{	
	Pickup* tempPickup = new Pickup();
	tempPickup->Init(32, texture, renderer, xPos, yPos, 25);
	this->PickupList.push_back(tempPickup);
	SDL_Log("[%s] Pickup Spawned... xPos: %i, yPos: %i", getTime(), xPos,yPos, 25);	
}

void PickupManager::Update(class PlayerController &playerController)
{
	int i = 0;
	for (auto& element : PickupList)
	{		
		element->Update(playerController);	

		int collisionType = DetectCollisionPlayer(playerController, element->maxX, element->maxY, element->minX, element->minY);

		if (collisionType != 0)
		{
			PickupList.erase(PickupList.begin() + i);
			if (element->type == 0)
			{
				SDL_Log("[%s] Coin Collected... xPos: %i, yPos: %i", getTime(), element->minX, element->minY);
				playerController.ScoreGained(element->value);
			}
			else
			{
				SDL_Log("[%s] Heart Collected... xPos: %i, yPos: %i", getTime(), element->minX, element->minY);
				playerController.HealthGained(element->value);
			}
			collected = true;
			break;				
		}
		i++;
	}
}

void PickupManager::Render()
{
	//Loops through PickupList and renders the sprites for all EnemyControllers within
	for (auto& element : PickupList)
	{
		element->Render(renderer);
	}
}

//No Collision = 0, Horizontal Collision = 1, Vertical Collision = 2
int PickupManager::DetectCollisionPlayer(class PlayerController &playerController, int maxX, int maxY, int minX, int minY)
{
	bool xCollision = false;
	bool yCollision = false;

	if (((maxY > playerController.minY && minY < playerController.maxY) && (maxX > playerController.minX && minX < playerController.maxX)))
	{
		if (!yCollision && ((maxY >= playerController.minY && minY < playerController.minY) || (maxY <= playerController.maxY && minY > playerController.maxY)))
		{
			return 1;
		}
		if (!xCollision && ((maxX >= playerController.minX && minX < playerController.minX) || (minX <= playerController.maxX && maxX > playerController.maxX)))
		{
			return 1;
		}
	}
	return 0;
}

PickupManager::~PickupManager() 
{
	for (auto& element : PickupList)
	{
		delete element;
	}
}
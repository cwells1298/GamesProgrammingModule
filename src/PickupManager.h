#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>   // functions for using vectors
#include "SDL_mixer.h"

#include "Pickup.h"
#include "PlayerController.h"
#include "Logging.h"

using namespace std;

class PickupManager
{
private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;
public:
	PickupManager();

	void Init(SDL_Texture* aTexture, SDL_Renderer* aRenderer);

	void AddPickups(int xPos, int yPos);

	void Update(class PlayerController &playerController);

	void Render();

	int PickupManager::DetectCollisionPlayer(class PlayerController &playerController, int maxX, int maxY, int minX, int minY);

	~PickupManager();

	bool collected = false;
	vector<class Pickup*> PickupList;
	vector<class Pickup*>::iterator it;
};
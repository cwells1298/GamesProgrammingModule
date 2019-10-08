#pragma once#pragma once
#include <iostream>
#include <SDL.h>
#include "PickupManager.h"
#include "Logging.h"

class Pickup
{
private:
	//Used for the positioning and sizing of the Pickup
	int xPos;
	int yPos;
	int width;
	int height;

	//Used for the source position of the Pickup
	int frameWidthPixels = 32;
	int frameHeightPixels = 32;

	//Used to locate and display the required Pickup
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	int animLast = 0;
	int animFramesIndex = 0;
	int animDelay = 125;
	int maxFrame = 9;
	int ticks = 0;

public:

	Pickup();

	void Init(int size, SDL_Texture* aTexture, SDL_Renderer* aRenderer, int eX, int eY, int val);
	
	void Update(class PlayerController &playerController);

	void Render(SDL_Renderer* aRenderer);

	~Pickup();

	//Used for collision detection
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;

	int value = 0;

	//0 = Coin, 1 = Potion
	int type = 0;

};
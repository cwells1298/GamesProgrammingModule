#include "Pickup.h"

using namespace std;

Pickup::Pickup() {}

//Assigns initial values to the relevant variable
void Pickup::Init(int size, SDL_Texture* aTexture, SDL_Renderer* aRenderer, int eX, int eY, int val)
{
	srand(time(0));

	xPos = eX;
	yPos = eY;
	width = size;
	height = size;
	//The initial position and size of the Pickup

	minX = xPos + 5;
	maxX = minX + width - 10;
	minY = yPos + 5;
	maxY = minY + height - 10;

	value = val;

	srcRect = { 0, 0, 32,32 };
	dstRect = { xPos, yPos, width, height };

	texture = aTexture;

	if (rand() % 4 < 3)
	{
		type = 0;
	}
	else
	{
		type = 1;
	}
}

void Pickup::Update(class PlayerController &playerController)
{
	ticks = SDL_GetTicks();
	if (ticks > animLast + animDelay)
	{
		animFramesIndex++;
		animLast = ticks;
	}

	srcRect = { 32 * (animFramesIndex % maxFrame), type * 32, 32,32 };		
}

void Pickup::Render(SDL_Renderer* aRenderer)
{
	SDL_RenderCopy(aRenderer, texture, &srcRect, &dstRect);
}

Pickup::~Pickup() {}
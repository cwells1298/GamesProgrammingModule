#pragma once
#include <iostream>
#include <SDL.h>


#include "Logging.h"
class Sprite
{
private:
	//Used for the positioning and sizing of the sprite
	int xPos;
	int yPos;
	int width;
	int height;

	//Used for the source position of the sprite
	int frameWidthPixels = 32;
	int frameHeightPixels = 32;

	//Used to locate and display the required sprite
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	int rotation = 0;
	int angle = 0;

public:

	Sprite();

	void Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, int sourceX, int sourceY, int w, int h);

	void Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, int sourceX, int sourceY, int w, int h, int a, int r);

	void Update(int eX, int eY);

	void Render(SDL_Renderer* aRenderer);

	~Sprite();


	//Used for collision detection
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
};
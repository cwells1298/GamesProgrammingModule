#pragma once
#include <iostream>
#include <SDL.h>

#include "Sprite.h"
#include "Vector2D.h"
#include "ParticleManager.h"
#include "Logging.h"

class Particle
{
private:
	int xPos = 0;
	int yPos = 0;
	int size = 0;
	SDL_Texture* texture;	
	int initialAngle = 0;

	//Deals with how the particles disperse and react to gravity
	Vector2D velocity;
	int xDeviation = 20;
	int yDeviation = 20;
	int gravityScale = 1;
	int rotationScale = 10;

	Sprite particleSprite;
public:
	Particle();

	void Init(int s, SDL_Texture* aTexture, SDL_Renderer* aRenderer,int eX,int eY);

	void Update(SDL_Window* window);

	void Render(SDL_Renderer* aRenderer);

	~Particle();

	//Used for collision detection
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
};


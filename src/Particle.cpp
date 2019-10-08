#include "Particle.h"

Particle::Particle(){}

void Particle::Init(int s, SDL_Texture* aTexture, SDL_Renderer* aRenderer, int eX, int eY)
{
	size = s;
	texture = aTexture;
	xPos = eX;
	yPos = eY;

	velocity.x = -(0.5 * xDeviation) + rand() % xDeviation;
	velocity.y = -rand() % yDeviation;
	initialAngle =  10 * rand() % 36;

	//velocity.x = 1;

	if (velocity.x < 0)
	{
		rotationScale = -rotationScale;
	}

	minX = xPos + 5;
	maxX = minX + size - 10;
	minY = yPos + 5;
	maxY = minY + size - 10;

	particleSprite.Init(aRenderer,aTexture,xPos,yPos,0,0,size,size, initialAngle, rotationScale);
}

void Particle::Update(SDL_Window* window)
{
	xPos += velocity.x;
	yPos += velocity.y;

	if (velocity.y <= 10)
	{
		velocity.y += gravityScale;
	}	

	minX = xPos + 5;
	maxX = minX + size - 10;
	minY = yPos + 5;
	maxY = minY + size - 10;

	particleSprite.Update(xPos, yPos);
}

void Particle::Render(SDL_Renderer* aRenderer)
{
	particleSprite.Render(aRenderer);
}

Particle::~Particle() {}
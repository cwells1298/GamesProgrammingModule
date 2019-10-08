#include "Sprite.h"

using namespace std;

Sprite::Sprite(){}

//Assigns initial values to the relevant variable
void Sprite::Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, int sourceX, int sourceY, int w, int h)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	//The initial position and size of the sprite
		
	minX = xPos;
	maxX = minX + width;
	minY = yPos;
	maxY = minY + height;

	srcRect = { sourceX * 32, sourceY * 32, 32,32};
	dstRect = { xPos, yPos, width, height };

	texture = aTexture;
}

//Assigns initial values to the relevant variable (includes rotation values)
void Sprite::Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int x, int y, int sourceX, int sourceY, int w, int h, int a, int r)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	//The initial position and size of the sprite

	rotation = r;
	angle = a;

	minX = xPos;
	maxX = minX + width;
	minY = yPos;
	maxY = minY + height;

	srcRect = { sourceX * 32, sourceY * 32, 32,32 };
	dstRect = { xPos, yPos, width, height };

	texture = aTexture;
}

void Sprite::Update(int eX,int eY)
{
	xPos = eX;
	yPos = eY;	

	dstRect = { xPos, yPos, width, height };
	angle = (angle + rotation) % 360;
}

void Sprite::Render(SDL_Renderer* aRenderer)
{
	//Renders the sprite to screen
	//SDL_Rect* r = new SDL_Rect();
	//r->x = xPos; r->y = yPos, r->w = 32;r->h = 32;
	//SDL_SetRenderDrawColor(aRenderer, 0, 0, 255, 255);
	//SDL_RenderFillRect(aRenderer, r);
	SDL_RenderCopyEx(aRenderer, texture, &srcRect, &dstRect, angle, NULL, SDL_FLIP_NONE);
}

Sprite::~Sprite() {}
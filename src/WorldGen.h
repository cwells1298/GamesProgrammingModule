#pragma once
#include <iostream> // functions for printing things out
#include <vector>   // functions for using vectors
#include "SDL.h"	// functions for SDL

#include "Sprite.h"
#include "Logging.h"

using namespace std;

class WorldGen
{
private:
	//Get source coordinates for sprites
	int srcX;
	int srcY;
	bool borderField = false;

public:
	WorldGen();

	void Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int width, int height);

	void Render(SDL_Renderer* aRenderer);

	void Reset();

	~WorldGen();

	vector<class Sprite*> borderList;
	vector<class Sprite*> levelBlockList;
};

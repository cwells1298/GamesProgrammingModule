#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>   // functions for using vectors

#include "Particle.h"
#include "WorldGen.h"
#include "Logging.h"

using namespace std;

class ParticleManager
{
private:
	int particleNumber;	
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	int x;
	int y;
public:
	ParticleManager();

	void Init(int pN, SDL_Texture* aTexture, SDL_Renderer* aRenderer);

	void AddParticles(int xPos, int yPos);

	void Update(SDL_Window* window, WorldGen &worldGen);

	void Render();

	int DetectCollisionWorld(WorldGen &worldGen, int maxX, int maxY, int minX, int minY);

	~ParticleManager();

	vector<class Particle*> particleList;
	vector<class Particle*>::iterator it;
};

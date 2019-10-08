#include "ParticleManager.h"

ParticleManager::ParticleManager(){}

void ParticleManager::Init(int pN, SDL_Texture* aTexture,SDL_Renderer* aRenderer)
{
	particleNumber = pN;
	texture = aTexture;
	renderer = aRenderer;
}

void ParticleManager::AddParticles(int xPos, int yPos)
{
	SDL_Log("[%s] Adding Particles", getTime());
	x = xPos;
	y = yPos;
	for (int i = 0; i < particleNumber; i++)
	{
		Particle* tempParticle = new Particle();
		tempParticle->Init(32, texture, renderer,x,y);
		this->particleList.push_back(tempParticle);
		SDL_Log("[%s] Particle Spawned", getTime());
	}	
}

void ParticleManager::Update(SDL_Window* window, WorldGen &worldGen)
{
	int i = 0;
	for (auto& element : particleList)
	{
		element->Update(window);

		if (DetectCollisionWorld(worldGen, element->maxX, element->maxY, element->minX, element->minY))
		{
			particleList.erase(particleList.begin() + i);
			SDL_Log("[%s] Particle Despawned", getTime());
			break;
		}
		i++;
	}
}

void ParticleManager::Render()
{
	//Loops through particleList and renders the sprites for all EnemyControllers within
	for (auto& element : particleList)
	{
		element->Render(renderer);
	}
}

int ParticleManager::DetectCollisionWorld(WorldGen &worldGen, int maxX, int maxY, int minX, int minY)
{
	bool yCollision = false, xCollision = false;

	//Detects enemy collision with environment
	for (auto& element : worldGen.borderList)
	{
		if (((maxY >= element->minY && minY <= element->maxY) && (maxX >= element->minX && minX <= element->maxX)))
		{
			return 1;
		}
	}

	return 0;
}

ParticleManager::~ParticleManager() {}
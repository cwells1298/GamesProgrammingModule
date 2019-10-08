#include "WorldGen.h"

WorldGen::WorldGen() {}

void WorldGen::Init(SDL_Renderer* aRenderer, SDL_Texture* aTexture, int width, int height)
{
	//Creates a sprite border for the initial window size
	for (int i = 0; i < width; i+=64)
	{				
		for (int j = 0; j < height; j+=64)
		{
			Sprite* tempSprite = new Sprite();
			
			if (i > width - 64)
			{
				i = width - 64;
			}
			if (j > height - 64)
			{
				j = height - 64;
			}

			//Determines which part of the border is needed, i.e. corners or sides
			if (i == 0 && j == 0)
			{
				//Top Left Corner
				srcX = 4;
				srcY = 0;
				borderField = true;
			}
			else if (i == 0 && j == height - 64)
			{
				//Bottom Left Corner
				srcX = 5;
				srcY = 2;
				borderField = true;
			}
			else if (i == width - 64 && j == 0)
			{
				//Top Right Corner
				srcX = 8;
				srcY = 0;
				borderField = true;
			}
			else if (i == width - 64 && j == height - 64)
			{
				//Bottom Right Corner
				srcX = 6;
				srcY = 2;
				borderField = true;
			}
			else if (i == 0 || i == width - 64)
			{
				//Vertical Side Borders
				srcX = 3;
				srcY = 2;
				borderField = true;
			}			
			else if (j == 0 || j == height - 64)
			{
				//Horizontal Side Borders
				srcX = 3;
				srcY = 1;
				borderField = true;
			}
			else
			{
				borderField = false;
			}
		

			if (borderField == true)
			{
				//Adds the new border sprite to the vector container
				tempSprite->Init(aRenderer, aTexture, i, j, srcX, srcY,64,64);
				this->borderList.push_back(tempSprite);
			}
			else
			{
				//Deletes tempSprite if not being used
				delete tempSprite;
			}
		}		
	}


	for (int i = 0; i < 10; i++)
	{
		Sprite* tempSprite = new Sprite();
		//Adds the new level sprite to the vector container
		if (i < 9)
		{
			tempSprite->Init(aRenderer, aTexture, 64 + i * 64, 192, 3, 1, 64, 64);
		}
		else
		{
			tempSprite->Init(aRenderer, aTexture, 64 + i * 64, 192, 3, 3, 64, 64);
		}
		this->borderList.push_back(tempSprite);
	}

	for (int i = 0; i < 10; i++)
	{
		Sprite* tempSprite = new Sprite();
		//Adds the new level sprite to the vector container
		if (i < 9)
		{
			tempSprite->Init(aRenderer, aTexture, width - (128 + i * 64), 320, 3, 1, 64, 64);
		}
		else
		{
			tempSprite->Init(aRenderer, aTexture, width - (128 + i * 64), 320, 0, 3, 64, 64);
		}

		this->borderList.push_back(tempSprite);
	}		

	for (int i = 0; i < 10; i++)
	{
		Sprite* tempSprite = new Sprite();
		//Adds the new level sprite to the vector container
		if (i < 9)
		{
			tempSprite->Init(aRenderer, aTexture, 64 + i * 64, 448, 3, 1, 64, 64);
		}
		else
		{
			tempSprite->Init(aRenderer, aTexture, 64 + i * 64, 448, 3, 3, 64, 64);
		}
		
		this->borderList.push_back(tempSprite);
	}

	for (int i = 0; i < 10; i++)
	{
		Sprite* tempSprite = new Sprite();
		//Adds the new level sprite to the vector container
		if (i < 9)
		{
			tempSprite->Init(aRenderer, aTexture, width - (128 + i * 64), 578, 3, 1, 64, 64);
		}
		else
		{
			tempSprite->Init(aRenderer, aTexture, width - (128 + i * 64), 578, 0, 3, 64, 64);
		}

		this->borderList.push_back(tempSprite);
	}
}

void WorldGen::Render(SDL_Renderer* aRenderer)
{
	//Loops through borderList to render all of the contained sprites
	for (auto& element : borderList)
	{
		element->Render(aRenderer);
	}
	////Loops through levelBlockList to render all of the contained sprites
	//for (auto& element : levelBlockList)
	//{
	//	element->Render(aRenderer);
	//}
}

void WorldGen::Reset()
{
	borderList.clear();
	//levelBlockList.clear();
}

WorldGen::~WorldGen() 
{
	//Deletes all subobjects (Sprites) before deleting this object
	for (auto& element : borderList)
	{
		delete element;
	}
	////Deletes all subobjects (Sprites) before deleting this object
	//for (auto& element : levelBlockList)
	//{
	//	delete element;
	//}

}
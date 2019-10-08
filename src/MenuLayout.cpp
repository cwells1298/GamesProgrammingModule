#include "MenuLayout.h"

Layout::Layout() {}

void Layout::Init(SDL_Window* window, int alphaVal, int type)
{
	int itemNum = 0;

	for (int i = 0; i < 4; i++)
	{
		menuItems.push_back(titleText);
	}
	
	for (auto& element : menuItems)
	{
		element = new ScreenText(window, "content/TTF/IndieFlower.ttf");
			
		if (itemNum == 0)
		{
			//Set Font Colour
			element->setColor(139, 125, 107, 255);
			if (type == 0)
			{				
				//Set text to render, location and size
				element->setText("Play Game", 96, 128 * ( itemNum + 1), 96);
			}
			else
			{
				//Set text to render, location and size
				element->setText("Continue", 96, 128 * ( itemNum + 1), 96);
			}
			optionXpos[0] = 96;
			optionYpos[0] = 128;
			optionWidth[0] = 300;
			optionHeight[0] = 90;
		}
		else if (itemNum == 1)
		{
			//Set Font Colour
			element->setColor(255, 235, 205, 255);
			if(type == 0)
			{
				//Set text to render, location and size
				element->setText("Options", 96, 128 * ( itemNum + 1), 96);
			}
			else
			{
				//Set text to render, location and size
				element->setText("Options", 96, 128 * ( itemNum + 1), 96);
			}
			optionXpos[1] = 96;
			optionYpos[1] = 256;
			optionWidth[1] = 300;
			optionHeight[1] = 96;
		}
		else if (itemNum == 2)
		{
			//Set Font Colour
			element->setColor(255, 235, 205, 255);
			if (type == 0)
			{
				//Set text to render, location and size
				element->setText("Exit Game", 96, 128 * ( itemNum + 1), 96);
			}
			else
			{
				//Set text to render, location and size
				element->setText("Exit To Menu", 96, 128 * ( itemNum + 1), 96);
			}
			optionXpos[2] = 96;
			optionYpos[2] = 384;
			optionWidth[2] = 300;
			optionHeight[2] = 96;
		}

		itemNum++;
	}
}

void Layout::InputReceived()
{
	int i = 0;
	for (auto& element : menuItems)
	{
		if (i == currentlySelected)
		{
			//Set Font Colour
			element->setColor(139, 125, 107, 255);
		}
		else
		{
			//Set Font Colour
			element->setColor(255, 235, 205, 255);
		}
		i++;
	}
}

void Layout::Update()
{
	SDL_PumpEvents();
	SDL_GetMouseState(&mouseX, &mouseY);
	for (int i = 0; i < 3; i++)
	{
		if ((mouseX <= optionXpos[i] + optionWidth[i]) && (mouseX >= optionXpos[i]) && (mouseY <= optionYpos[i] + optionHeight[i]) && (mouseY >= optionYpos[i]))
		{						
			currentlySelected = i;
			SDL_Log("MouseOn %i", i);
			InputReceived();
		}
	}

}

void Layout::Render(SDL_Renderer* aRenderer)
{
	SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, alphaValue);

	for (auto& element : menuItems)
	{
		//Render Text
		element->render();
	}

}
Layout::~Layout() {}
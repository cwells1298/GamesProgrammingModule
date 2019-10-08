#include "LoadingScreen.h"

LoadScreen::LoadScreen() {}

void LoadScreen::Init(SDL_Window* window)
{
	currentFrame[0] = "Conor Wells - 14589397";
	currentFrame[1] = "Games Programming";
	currentFrame[2] = "CGP2011M";

	titleText = new ScreenText(window, "content/TTF/IndieFlower.ttf");

	//Set Font Colour
	titleText->setColor(255, 235, 205, 0);
	//
	////Set text to render, location and size
	//titleText->setText(currentFrame[0], SDL_GetWindowSurface(window)->w - (75 * currentFrame[0].length), SDL_GetWindowSurface(window)->h - 75, 150);
	titleText->setText(currentFrame[0], 50, 50, 150);
}

void LoadScreen::Update(SDL_Window* window)
{
	frameTicks = SDL_GetTicks();

	if (frameTicks > frameLast + frameDelay)
	{
		frameLast = frameTicks;
		frameNum++;	
		if (frameNum < 3)
		{
			//Set text to render, location and size
	/*		titleText->setText(currentFrame[frameNum], SDL_GetWindowSurface(window)->w - (75 * currentFrame[frameNum].length), SDL_GetWindowSurface(window)->h - 75, 150);*/
			titleText->setText(currentFrame[frameNum], 50, 50, 150);
			alphaLast = SDL_GetTicks();
		/*	currentFrame[frameNum]*/
		}
		else
		{
			finished = true;
		}
	}
	
	alphaTicks = SDL_GetTicks();

	if (frameTicks - frameLast < 1000)
	{
		if ((alphaTicks > alphaLast + alphaDelay))
		{
			alphaLast = alphaTicks;
			alphaValue++;
			//Set Font Colour
			titleText->setColor(255, 235, 205, alphaValue);
		}
	}

	if (frameTicks - frameLast >= 1000 && frameTicks - frameLast < 2000)
	{
		alphaLast = alphaTicks;		
		alphaValue = 255;
		//Set Font Colour
		titleText->setColor(255, 235, 205, alphaValue);
	}

	if (frameTicks - frameLast >= 2000)
	{
		if ((alphaTicks > alphaLast + alphaDelay))
		{
			alphaLast = alphaTicks;
			alphaValue--;
			//Set Font Colour
			titleText->setColor(255, 235, 205, alphaValue);
		}
	}	
}

void LoadScreen::Render(SDL_Renderer* aRenderer)
{
	SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, 255);
	//Render Text
	titleText->render();
	SDL_RenderPresent(aRenderer);
	SDL_RenderClear(aRenderer);
}

LoadScreen::~LoadScreen() {}
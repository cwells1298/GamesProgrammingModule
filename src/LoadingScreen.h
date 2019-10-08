#pragma once
//C++ Headers
#include <iostream> // functions for printing things out
#include <string>   // functions for strings
#include <vector>

//#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"

using namespace std;

class LoadScreen
{
private:
	ScreenText* titleText;
	//0 = Main Menu, 1 = Pause Menu
	int alphaValue = 0;
	string currentFrame[3];
	int frameNum = 0;
	
	//Determines how fast the various frames should be scrolled through
	int frameTicks = 0;
	int frameDelay = 3000;
	int frameLast = 0;
	//Determines how fast the alpha value of the text should change
	int alphaTicks = 0;
	int alphaDelay = 4;
	int alphaLast = 0;

public:
	LoadScreen();
	void Init(SDL_Window* window);
	void Update(SDL_Window* window);
	void Render(SDL_Renderer* aRenderer);
	~LoadScreen();
	
	bool finished = false;
};
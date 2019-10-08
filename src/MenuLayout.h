#pragma once
//C++ Headers
#include <iostream> // functions for printing things out
#include <string>   // functions for strings
#include <vector>

//#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"

using namespace std;

class Layout
{
private:
	vector<ScreenText*> menuItems;
	ScreenText* titleText;

	int optionXpos[3];
	int optionYpos[3];
	int optionWidth[3];
	int optionHeight[3];

	int mouseX;
	int mouseY;

	//0 = Main Menu, 1 = Pause Menu
	int menuType = 0;
	int alphaValue = 0;
public:
	Layout();
	void Init(SDL_Window* window, int alphaVal, int type);
	void InputReceived();
	void Update();
	void Render(SDL_Renderer* aRenderer);
	~Layout();
	int currentlySelected = 0;
};
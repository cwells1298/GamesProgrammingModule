#pragma once
//C++ Headers
#include <iostream> // functions for printing things out
#include <string>   // functions for strings
#include <vector>

//#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"
#include "PlayerController.h"

using namespace std;

class UI
{
private:
	ScreenText* healthText;
	ScreenText* scoreText;
	vector<ScreenText*> logItems;

	string health = "";
	string score = "";

	//Location of Player Health Text
	int healthX = 0;
	int healthY = 0;

	//Location of Player Health Text
	int scoreX = 0;
	int scoreY = 0;

	int logFontSize = 32;
	int UIFontSize = 48;

	bool logShown = false;

public:
	UI();
	void Init(SDL_Window* window);
	void InputReceived(SDL_Event event);
	void Update(class PlayerController &player);
	void Render(SDL_Renderer* aRenderer);
	~UI();
	int playerScore = 0;
	int playerHealth = 0;
};
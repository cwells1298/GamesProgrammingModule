#pragma once
//C++ Headers
#include <iostream> // functions for printing things out
#include "SDL.h"	// functions for SDL
#include <string>
#include <fstream> //Include file reading library
#include "json.hpp" //Include file reading library
#include "Logging.h"
//Make it so we don’t have to type out nlohmann::json every time we want to use a function in the library
using json = nlohmann::json;

using namespace std;

class KeyboardControls
{
public:
	//Used to allow for control config
	SDL_Scancode moveLeft;
	SDL_Scancode moveRight;
	SDL_Scancode jump;
	SDL_Scancode attack;
	SDL_Keycode pause;
	SDL_Keycode selectMenuItem;
	SDL_Keycode scrollMenuItemUp;
	SDL_Keycode scrollMenuItemDown;
	SDL_Keycode toggleFullscreen;

	KeyboardControls();

	void LoadKeys(const string& path);
		
	~KeyboardControls();
};
#include "Controls.h"

KeyboardControls::KeyboardControls() {}

void KeyboardControls::LoadKeys(const std::string& path)
{
	//The file stream in (to open)
	ifstream inStream;

	//Open the file
	inStream.open(path, std::ios::in);

	//An empty json object to read json data into
	json jsonObject;

	//Read from the file into the json object.. let json.hpp parse it for us
	inStream >> jsonObject;

	SDL_Log("[%s] JSON file read in", getTime());

	//Get the fire and jump keys.. set them for this instance
	this->moveLeft = SDL_GetScancodeFromName(jsonObject["moveLeft"].get<std::string>().c_str());
	SDL_Log("[%s] Move Left Read", getTime());
	this->moveRight = SDL_GetScancodeFromName(jsonObject["moveRight"].get<std::string>().c_str());
	SDL_Log("[%s] Move Right Read", getTime());
	this->attack = SDL_GetScancodeFromName(jsonObject["attack"].get<std::string>().c_str());
	SDL_Log("[%s] Attack Read", getTime());
	this->jump = SDL_GetScancodeFromName(jsonObject["jump"].get<std::string>().c_str());
	SDL_Log("[%s] Jump Read", getTime());
	this->pause = SDL_GetKeyFromName(jsonObject["pause"].get<std::string>().c_str());
	SDL_Log("[%s] Pause Read", getTime());
	this->selectMenuItem = SDL_GetKeyFromName(jsonObject["selectMenuItem"].get<std::string>().c_str());
	SDL_Log("[%s] Select Menu Item Read", getTime());
	this->scrollMenuItemUp= SDL_GetKeyFromName(jsonObject["scrollMenuItemUp"].get<std::string>().c_str());
	SDL_Log("[%s] Scroll Menu Item Up Read", getTime());
	this->scrollMenuItemDown = SDL_GetKeyFromName(jsonObject["scrollMenuItemDown"].get<std::string>().c_str());
	SDL_Log("[%s] Scroll Menu Item Down Read", getTime());
	this->toggleFullscreen = SDL_GetKeyFromName(jsonObject["toggleFullscreen"].get<std::string>().c_str());
	SDL_Log("[%s] Toggle Fullscreen Read", getTime());

	//And close the file
	inStream.close();

	SDL_Log("[%s] JSON file closed", getTime());
}

KeyboardControls::~KeyboardControls() {}
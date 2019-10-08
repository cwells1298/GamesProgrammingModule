#include "InGameUI.h"

UI::UI() {}

void UI::Init(SDL_Window* window)
{
	healthX = 70;
	healthY = 40;
	scoreX = 70;
	scoreY = 70;

	healthText = new ScreenText(window, "content/TTF/IndieFlower.ttf");
	scoreText = new ScreenText(window, "content/TTF/IndieFlower.ttf");
	
	//Set Font Colour
	healthText->setColor(47, 79, 79, 255);
	scoreText->setColor(47, 79, 79, 255);
	
	health = "Health: 0 / 0";
	score = "Score: 0";

	healthText->setText(health, healthX, healthY, UIFontSize);
	scoreText->setText(score, playerScore, scoreX, scoreY, UIFontSize);
}

void UI::InputReceived(SDL_Event event)
{	
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_t)
		{
			logShown = !logShown;
		}
	}	
}

void UI::Update(class PlayerController &player)
{
	health = "Health: " + to_string(player.currentHealth) + "/" + to_string(player.maxHealth);
	score = "Score: " + to_string(player.playerScore);

	healthText->setText(health, healthX, healthY, UIFontSize);
	scoreText->setText(score, scoreX, scoreY, UIFontSize);
}

void UI::Render(SDL_Renderer* aRenderer)
{
	//Render Text
	healthText->render();
	scoreText->render();
}

UI::~UI() {}
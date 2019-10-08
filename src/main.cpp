/**
 * @file   main.cpp
 * @author Conor Wells (cwells1298@gmail.com)
 * @brief  The main entry point for my Games Programming Assignment
*/

//C++ Headers
#include <iostream> // functions for printing things out
#include <string>   // functions for strings
#include <ctime>	// functions for time
#include "time.h"
#include "SDL.h"	// functions for SDL
#include "SDL_image.h"//Image functions for SDL
#include "SDL_mixer.h"
#include "Controls.h"

#define STB_TRUETYPE_IMPLEMENTATION

//Self Made Header Files
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "WorldGen.h"
#include "ParticleManager.h"
#include "PickupManager.h"
#include "MenuLayout.h"
#include "InGameUI.h"
#include "LoadingScreen.h"
#include "Logging.h"

//Window width and window height
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

using namespace std;

void FixedUpdate(SDL_Renderer* renderer, SDL_Window* window, bool &paused, bool &running);
void Render(SDL_Renderer* renderer, bool &paused);
void Input(bool &running, SDL_Window* window, bool &paused);
void MainMenu(SDL_Renderer* aRenderer, SDL_Window* window, bool &running, int alphaVal);
void GameRunning(SDL_Renderer* aRenderer, SDL_Window* window, bool &running, bool &menu);
void RunAudio();
void logOverride(void* userdata, int category, SDL_LogPriority priority, const char* message);

char* getTime(void);

//Declaring player1 as a global variable
PlayerController player1;

EnemySpawner enemySpawn1;

WorldGen worldGen;

//Particle Manager for enemy deaths
ParticleManager ePManager;

//Pickup Manager for pickups
PickupManager pickUpManager;

Layout mainMenuLayout;
Layout pauseMenuLayout;

UI inGameUI;

bool paused = false;
bool startMenu = true;
bool fullscreen = false;

SDL_Joystick* joystick1 = NULL;

Mix_Music *SOUND_aBckMusic = NULL;
Mix_Chunk *SOUND_aPickupCollected = NULL;
Mix_Chunk *SOUND_aPlayerJump = NULL;
Mix_Chunk *SOUND_aEnemyHurt = NULL;
Mix_Chunk *SOUND_aMenuSelected = NULL;

//Determines how often fixed update should be called
int updateTicks = 0;
//FixedUpdate called every 25ms
int updateDelay = 25;
int updateLast = 0;

int lastInputTick = 0;
int lastRenderTick = 0;
int lastUpdateTick = 0;
int lastGameLoopTick = 0;

bool muteSoundEffects = false;
int musicVolume = 4;
int soundEffectVolume = 16;
bool muteMusic = false;
bool muteSound = false;
bool menuSoundTrigger = false;

KeyboardControls keyboardControls;

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char *argv[])
{	
	//Detects whether the game has been closed
	bool running = true;

	//Used to determine how many ticks have passed since the last frame
	int currentTicks = 0;
	int lastTicks = 0;
	int passedTicks = 0;
	int frames = 0;

	//Used to store the current dimensions of the game window
	int windowWidth;
	int windowHeight;
	
	srand(time(0));

	//Initialises SDL Functions
	{
		IMG_Init(SDL_INIT_EVERYTHING);

		//Firstly initialise SDL.. log an error if we can't open it
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			//Log an error
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "An error occurred when initialising SDL.. closing..");

			//Return.. exit the program
			return -1;
		}

		//Detects whether there are any controllers connected
		if (SDL_NumJoysticks() < 1)
		{
			SDL_Log("[%s] No Controller Detected", getTime());			
		}
		else
		{
			if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
			{
				SDL_Log("SDL could access Joystick! SDL Error: %s\n", SDL_GetError());
			}
			SDL_Log("[%s] Controller Detected", getTime());
			joystick1 = SDL_JoystickOpen(0);
		}
	
		//Initialises all sounds
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			printf("Warning: Audio has not been found! \n");
		}
		else
		{
			//Initialises music
			{
				SOUND_aBckMusic = Mix_LoadMUS("content/Audio/Music/PixelRiver.wav");
				Mix_VolumeMusic(musicVolume); //The volume for the music (0-255)
			}	

			//Initialises Sound Effects
			{
				SOUND_aPickupCollected = Mix_LoadWAV("./content/Audio/SoundEffects/Coin.wav");
				Mix_VolumeChunk(SOUND_aPickupCollected, soundEffectVolume); //The Volume of this sound (0-255)
				SOUND_aEnemyHurt = Mix_LoadWAV("./content/Audio/SoundEffects/EnemyHurt.wav");
				Mix_VolumeChunk(SOUND_aEnemyHurt, soundEffectVolume); //The Volume of this sound (0-255)
				SOUND_aPlayerJump = Mix_LoadWAV("./content/Audio/SoundEffects/Jump.wav");
				Mix_VolumeChunk(SOUND_aPlayerJump, soundEffectVolume); //The Volume of this sound (0-255)
				SOUND_aMenuSelected = Mix_LoadWAV("./content/Audio/SoundEffects/Menu.wav");
				Mix_VolumeChunk(SOUND_aMenuSelected, soundEffectVolume); //The Volume of this sound (0-255)
			}
		}
	}	

	SDL_Log("[%s] Game Started", getTime());
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,"[%s] Game Started", getTime());

	//Create the game window
	window = SDL_CreateWindow("Conor Wells; CGP2011M Games Programming; 14589397; Dungeon Crawler", 600, 200, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

	//Initialise the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_LogSetOutputFunction(&logOverride, NULL);
	
	SDL_Surface* surface;
	SDL_Texture* playerTexture;
	SDL_Texture* enemyTexture;
	SDL_Texture* borderTexture;
	SDL_Texture* enemyParticleTexture;
	SDL_Texture* coinTexture;
	//Loads all textures in
	{
		//Load in Player Spritesheet
		surface = IMG_Load("content/Sprites/Player.png");
		playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
		//Load in Enemy Spritesheet
		surface = IMG_Load("content/Sprites/Skeleton.png");
		enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
		//Load in Border Spritesheet
		surface = IMG_Load("content/Sprites/StoneBrick.png");
		borderTexture = SDL_CreateTextureFromSurface(renderer, surface);
		//Load in Particle Sprite
		surface = IMG_Load("content/Sprites/Bone.png");
		enemyParticleTexture = SDL_CreateTextureFromSurface(renderer, surface);
		//Load in Coin Spritesheet
		surface = IMG_Load("content/Sprites/Pickups.png");
		coinTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	
	//Initialises everything
	{
		mainMenuLayout.Init(window, 255, 0);
		pauseMenuLayout.Init(window, 128, 1);
		//Load in key bindings from json file
		keyboardControls.LoadKeys("../content/keyboardControls1.json");
	}

	//Deals with the games inital splash screen
	{
		LoadScreen* loadingScreen = new LoadScreen();
		loadingScreen->Init(window);

		SOUND_aBckMusic = Mix_LoadMUS("content/Audio/Music/PixelRiver.wav");
		//Play the music			
		Mix_PlayMusic(SOUND_aBckMusic, -1);
		SDL_Log("[%s] Playing Music", getTime());

		while (loadingScreen->finished == false)
		{
			loadingScreen->Update(window);
			loadingScreen->Render(renderer);
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
				{
					if ((event.key.keysym.sym == SDLK_ESCAPE && !event.key.repeat))
					{
						SDL_Log("[%s] Loading Screen Skipped", getTime());
						loadingScreen->finished = true;
					}
				}
			}
		}

		//delete loadingScreen;
	}
		
	bool gameRunStarted = false;

	//Reset Play Field
	{
		//worldGen = WorldGen();
		//player1 = PlayerController();
		//enemySpawn1.~EnemySpawner();
		//enemySpawn1 = EnemySpawner();

		//Initialise World Gen
		worldGen.Init(renderer, borderTexture, SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
		
		ePManager.Init(8, enemyParticleTexture, renderer);

		pickUpManager.Init(coinTexture, renderer);

		inGameUI.Init(window);
	}

	while (running == true)
	{			
		if (startMenu == true)
		{

			if (gameRunStarted == true)
			{
				SOUND_aBckMusic = Mix_LoadMUS("content/Audio/Music/PixelRiver.wav");
				Mix_PlayMusic(SOUND_aBckMusic, -1);
				SDL_Log("[%s] Playing Music", getTime());
				//Play the music
			}

			gameRunStarted = false;
			
			MainMenu(renderer, window, running, 255);
			startMenu = false;
		}	

		//Assigns values to window size
		windowWidth = SDL_GetWindowSurface(window)->w;
		windowHeight = SDL_GetWindowSurface(window)->h;

		if (SDL_WINDOWEVENT_RESIZED)
		{
			//Deletes all current border tiles to avoid a memory leak and remove unwanted tiles
			worldGen.Reset();
						
			SDL_SetWindowSize(window, windowWidth  - windowWidth % 64, windowHeight - windowHeight % 64);			

			//Creates border touching window edge but may not be properly aligned
			worldGen.Init(renderer, borderTexture, SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
		}

		//Increment frame number each loop
		frames++;					

		//Used to log how many ticks have passed since the last frame
		lastTicks = currentTicks;
		currentTicks = SDL_GetTicks();
		passedTicks = currentTicks - lastTicks;

		//SDL_Log("Frame Count: %i;  Ticks Passed: %i", frames, passedTicks);	

		if (running == true)
		{
			if (gameRunStarted == false)
			{
				//Play the music
				SOUND_aBckMusic = Mix_LoadMUS("content/Audio/Music/ValorOfHeroes.wav");
				Mix_PlayMusic(SOUND_aBckMusic, -1);
				gameRunStarted = true;
				//Initialise Player 1
				player1.Init(renderer, playerTexture, 64, 64);
				//Initialise EnemySpawner
				enemySpawn1.Init(window, renderer, enemyTexture, 2, 128, 130);
			}
		
			GameRunning(renderer, window, running, startMenu);
		}
	}

	//Closes down all game functions
	{
		SDL_Log("[%s] Game Closing", getTime());

		//Delay for 2 seconds
		SDL_Delay(2000);

		//Destroy our window -- free up memory that has been allocated to this window
		SDL_DestroyWindow(window);

		//Deallocate joystick memory
		SDL_JoystickClose(joystick1);
		joystick1 = NULL;

		//Deallocate sound memory
		Mix_FreeMusic(SOUND_aBckMusic);
		Mix_CloseAudio();

		SDL_Log("[%s] Game Closed", getTime());

		//Quit SDL2_Image
		IMG_Quit();

		//Tell SDL that we're quitting
		SDL_Quit();
	}

	return 0;
}

void FixedUpdate(SDL_Renderer* renderer, SDL_Window* window, bool &paused, bool &running)
{
	int updateTick = SDL_GetTicks();
	int ticksPassed = updateTick - lastUpdateTick;
	lastUpdateTick = updateTick;

	SDL_Log("[%s] Update Function Started: %i Ticks since last Update Function", getTime(), ticksPassed);

	Input(running,window,paused);	
		
	//Update Action of all enemies
	enemySpawn1.Update(window, worldGen, player1, pickUpManager, ePManager);
	RunAudio();

	ePManager.Update(window, worldGen);
	RunAudio();

	//Update Player Action
	player1.Update(window, worldGen, enemySpawn1);

	inGameUI.Update(player1);

	RunAudio();

	if (player1.dead == true)
	{
		return;
	}
	pickUpManager.Update(player1);
	RunAudio();
}

void RunAudio()
{
	if (pickUpManager.collected == true)
	{
		SDL_Log("[%s] Playing Coin.Wav", getTime());
		pickUpManager.collected = false;
		Mix_PlayChannel(-1, SOUND_aPickupCollected, 0);
	}
	if (player1.jumpAudioTrigger == true)
	{
		SDL_Log("[%s] Playing PlayerJump.Wav", getTime());
		player1.jumpAudioTrigger = false;
		Mix_PlayChannel(-1, SOUND_aPlayerJump, 0);
	}
	if (player1.enemyHurtAudioTrigger == true)
	{
		SDL_Log("[%s] Playing EnemyHurt.Wav", getTime());
		player1.enemyHurtAudioTrigger = false;
		Mix_PlayChannel(-1, SOUND_aEnemyHurt, 0);
	}
	if (player1.playerHurtAudioTrigger == true)
	{
		SDL_Log("[%s] Playing EnemyHurt.Wav", getTime());
		player1.playerHurtAudioTrigger = false;
		Mix_PlayChannel(-1, SOUND_aEnemyHurt, 0);
	}
	if (menuSoundTrigger == true)
	{
		SDL_Log("[%s] Playing Menu.Wav", getTime());
		menuSoundTrigger= false;
		Mix_PlayChannel(-1, SOUND_aMenuSelected, 0);
	}
}

void Render(SDL_Renderer* renderer, bool &paused)
{
	int renderTick = SDL_GetTicks();
	int ticksPassed = renderTick - lastRenderTick;
	lastRenderTick = renderTick;

	SDL_Log("[%s] Render Function Started: %i Ticks since last Render Function", getTime(), ticksPassed);

	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);

	if (paused == false)
	{
		//Clear the window
		SDL_RenderClear(renderer);

		//Render the Player
		worldGen.Render(renderer);

		//Render all enemies
		enemySpawn1.Render();

		ePManager.Render();

		//Render all pickups
		pickUpManager.Render();

		//Render the Player
		player1.Render(renderer);

		//Render ingame UI
		inGameUI.Render(renderer);
	}
	else
	{
		pauseMenuLayout.Render(renderer);
	}

	//Shows changes made
	SDL_RenderPresent(renderer);
}

void Input(bool &running, SDL_Window* window, bool &paused)
{
	int inputTick = SDL_GetTicks();
	int ticksPassed = inputTick - lastInputTick;
	lastInputTick = inputTick;

	SDL_Log("[%s] Input Function Started: %i Ticks since last Input Function", getTime(), ticksPassed);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:	//Determines whether the program should close
		{
			running = false;
		}

		//Determines keyboard input unrelated to a playerController
		case SDL_KEYDOWN:
			if ((event.key.keysym.sym == keyboardControls.pause && !event.key.repeat))
			{
				paused = !paused;
				if (paused == true)
				{
					SDL_Log("[%s] Game Paused", getTime());
				}
				else
				{
					SDL_Log("[%s] Game Unpaused", getTime());
				}
			}
			//Changes Sound Effect Volume
			if (((event.key.keysym.sym == SDLK_n) && !event.key.repeat))
			{			
				muteSoundEffects = !muteSoundEffects;
				if (muteSoundEffects == false)
				{
					SDL_Log("[%s] Sound Effects Unmuted", getTime());
					{
						Mix_VolumeChunk(SOUND_aPlayerJump, soundEffectVolume); //The Volume of this sound (0-255)
						Mix_VolumeChunk(SOUND_aPickupCollected, soundEffectVolume); //The Volume of this sound (0-255)
						Mix_VolumeChunk(SOUND_aEnemyHurt, soundEffectVolume); //The Volume of this sound (0-255)
					}
				}
				else
				{
					SDL_Log("[%s] Sound Effects Muted", getTime());
					{
						Mix_VolumeChunk(SOUND_aPlayerJump, 0); //The Volume of this sound (0-255)
						Mix_VolumeChunk(SOUND_aPickupCollected, 0); //The Volume of this sound (0-255)
						Mix_VolumeChunk(SOUND_aEnemyHurt, 0); //The Volume of this sound (0-255)
					}
				}
			}
			if (((event.key.keysym.sym == SDLK_PERIOD) && !event.key.repeat))
			{
				soundEffectVolume += 8;
				muteSoundEffects = false;
				if (soundEffectVolume > 255)
				{
					soundEffectVolume = 255;
				}
				SDL_Log("[%s] Sound Effect Volume: %i", getTime(), soundEffectVolume);
				{
					Mix_VolumeChunk(SOUND_aPlayerJump, soundEffectVolume); //The Volume of this sound (0-255)
					Mix_VolumeChunk(SOUND_aPickupCollected, soundEffectVolume); //The Volume of this sound (0-255)
					Mix_VolumeChunk(SOUND_aEnemyHurt, soundEffectVolume); //The Volume of this sound (0-255)
				}
			}
			if (((event.key.keysym.sym == SDLK_COMMA) && !event.key.repeat))
			{
				soundEffectVolume -= 8;
				muteSoundEffects = false;
				if (soundEffectVolume < 0)
				{
					soundEffectVolume = 0;
				}
				SDL_Log("[%s] Sound Effect Volume: %i", getTime(), soundEffectVolume);
				{
					Mix_VolumeChunk(SOUND_aPlayerJump, soundEffectVolume); //The Volume of this sound (0-255)
					Mix_VolumeChunk(SOUND_aPickupCollected, soundEffectVolume); //The Volume of this sound (0-255)
					Mix_VolumeChunk(SOUND_aEnemyHurt, soundEffectVolume); //The Volume of this sound (0-255)
				}
			}
			//Changes Music Volume
			if (((event.key.keysym.sym == SDLK_RIGHTBRACKET) && !event.key.repeat))
			{
				musicVolume += 8;
				muteMusic = false;
				if (musicVolume > 255)
				{
					musicVolume = 255;					
				}
				SDL_Log("[%s] Music Volume: %i", getTime(), musicVolume);
				Mix_VolumeMusic(musicVolume); //The volume for the music (0-255)
			}
			if (((event.key.keysym.sym == SDLK_LEFTBRACKET) && !event.key.repeat))
			{
				musicVolume -= 8;
				muteMusic = false;
				if (musicVolume < 0)
				{
					musicVolume = 0;			
				}
				SDL_Log("[%s] Music Volume: %i", getTime(), musicVolume);
				Mix_VolumeMusic(musicVolume); //The volume for the music (0-255)
			}
			if (((event.key.keysym.sym == SDLK_m) && !event.key.repeat))
			{		
				//If there is no music playing
				if (muteMusic == false)
				{
					Mix_VolumeMusic(0); //The volume for the music (0-255)
					muteMusic = true;
					SDL_Log("[%s] Music Muted", getTime());
				}
				else
				{			
					Mix_VolumeMusic(musicVolume); //The volume for the music (0-255)
					muteMusic = false;
					SDL_Log("[%s] Music Unmuted", getTime());
				}
		
			}
			if (((event.key.keysym.sym == keyboardControls.toggleFullscreen) && !event.key.repeat))
			{
				fullscreen = !fullscreen;
				SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);

				SDL_Log("[%s] Fullscreen Toggled", getTime());
			}
			break;
		case SDL_JOYBUTTONDOWN:
			if ((event.jbutton.button == 7))
			{
				if (paused == false)
				{
					paused = !paused;
					SDL_Log("[%s] Game Paused", getTime());
				}
			}

			if ((event.jbutton.button == 1))
			{
				if (paused == true)
				{
					paused = !paused;
					SDL_Log("[%s] Game Unpaused", getTime());
				}
			}
			SDL_Log("[%s] Button Pressed", getTime());
			break;

		default:
			break;
		}

		if (running == true && paused == false)
		{
			//Determines player 1's input parameters
			player1.Input(event, window, joystick1);
			inGameUI.InputReceived(event);
			RunAudio();
		}
	}
}

void MainMenu(SDL_Renderer* aRenderer, SDL_Window* window, bool &running, int alphaVal)
{
	bool onMenu = true;
	
	while (onMenu == true)
	{
		//Clear the window
		SDL_RenderClear(aRenderer);
		SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, alphaVal);
		

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:	//Determines whether the program should close
			{
				onMenu = false;
				running = false;
			}

			//Determines keyboard input unrelated to a playerController
			case SDL_KEYDOWN:
				if (((event.key.keysym.sym == keyboardControls.scrollMenuItemUp || event.key.keysym.sym == SDLK_w) && !event.key.repeat))
				{
					mainMenuLayout.currentlySelected--;
					if (mainMenuLayout.currentlySelected < 0)
					{
						mainMenuLayout.currentlySelected = 2;
					}
					mainMenuLayout.currentlySelected = mainMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
				}

				if (((event.key.keysym.sym == keyboardControls.scrollMenuItemDown || event.key.keysym.sym == SDLK_s) && !event.key.repeat))
				{
					mainMenuLayout.currentlySelected++;
					mainMenuLayout.currentlySelected = mainMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
				}

				if (((event.key.keysym.sym == keyboardControls.selectMenuItem) && !event.key.repeat))
				{
					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
					if (mainMenuLayout.currentlySelected == 0)
					{
						onMenu = false;
					}
					else if(mainMenuLayout.currentlySelected == 1)
					{

					}
					else if (mainMenuLayout.currentlySelected == 2)
					{
						onMenu = false;
						running = false;
					}
					menuSoundTrigger = true;
				}		

				if (((event.key.keysym.sym == SDLK_m) && !event.key.repeat))
				{
					//If there is no music playing
					if (Mix_PlayingMusic() == 0)
					{
						//Play the music
						Mix_PlayMusic(SOUND_aBckMusic, -1);
						SDL_Log("[%s] Playing Music", getTime());

					}
					else
					{
						Mix_HaltMusic();
						SDL_Log("[%s] Music Paused", getTime());
					}
				}
				
				if (((event.key.keysym.sym == keyboardControls.toggleFullscreen) && !event.key.repeat))
				{
					fullscreen = !fullscreen;
					SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);

					SDL_Log("[%s] Fullscreen Toggled", getTime());
				}

				break;					
			case SDL_JOYBUTTONDOWN:
				if ((event.jbutton.button == 0) || (event.jbutton.button == 7))
				{
					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
					if (mainMenuLayout.currentlySelected == 0)
					{
						onMenu = false;
					}
					else if (mainMenuLayout.currentlySelected == 1)
					{

					}
					else if (mainMenuLayout.currentlySelected == 2)
					{
						onMenu = false;
						running = false;
					}
					menuSoundTrigger = true;
				}
				SDL_Log("[%s] Button Pressed", getTime());

				if (event.jbutton.button == 6)
				{
					//If there is no music playing
					if (Mix_PlayingMusic() == 0)
					{
						//Play the music
						Mix_PlayMusic(SOUND_aBckMusic, -1);
						SDL_Log("[%s] Playing Music", getTime());

					}
					else
					{
						Mix_HaltMusic();
						SDL_Log("[%s] Music Paused", getTime());
					}
				}
				break;
			case SDL_JOYHATMOTION:				
				if (event.jhat.value == SDL_HAT_RIGHT)
				{
					mainMenuLayout.currentlySelected--;
					if (mainMenuLayout.currentlySelected < 0)
					{
						mainMenuLayout.currentlySelected = 2;
					}
					mainMenuLayout.currentlySelected = mainMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
				}
				if (event.jhat.value == SDL_HAT_DOWN)
				{
					mainMenuLayout.currentlySelected++;
					mainMenuLayout.currentlySelected = mainMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
				}
				SDL_Log("[%s]Hat Pressed", getTime());
				break;	
			case SDL_MOUSEBUTTONDOWN:
				SDL_Log("[%s] Current Menu Index %i", getTime(), mainMenuLayout.currentlySelected);
				if (mainMenuLayout.currentlySelected == 0)
				{
					onMenu = false;
				}
				else if (mainMenuLayout.currentlySelected == 1)
				{

				}
				else if (mainMenuLayout.currentlySelected == 2)
				{
					onMenu = false;
					running = false;
				}
				menuSoundTrigger = true;
				break;
			default:
				break;
			}
		}
		mainMenuLayout.Update();
		mainMenuLayout.InputReceived();
		mainMenuLayout.Render(aRenderer);
		RunAudio();
		//Shows changes made
		SDL_RenderPresent(aRenderer);
	}

}

void GamePaused(SDL_Renderer* aRenderer, SDL_Window* window, bool &running, int alphaVal)
{	
	//Clear the window
	SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, alphaVal);
	SDL_RenderClear(aRenderer);
	SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, alphaVal);
		
	SDL_Event event;
	if (paused ==  false)
	{
		SDL_Log("Paused");
	}

	while (paused == true)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:	//Determines whether the program should close				
				paused = false;
				running = false;
				break;
				//Determines keyboard input unrelated to a playerController
			case SDL_KEYDOWN:
				if (((event.key.keysym.sym == keyboardControls.scrollMenuItemUp || event.key.keysym.sym == SDLK_w) && !event.key.repeat))
				{
					pauseMenuLayout.currentlySelected--;
					if (pauseMenuLayout.currentlySelected < 0)
					{
						pauseMenuLayout.currentlySelected = 2;
					}
					pauseMenuLayout.currentlySelected = pauseMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
				}

				if (((event.key.keysym.sym == keyboardControls.scrollMenuItemDown || event.key.keysym.sym == SDLK_s) && !event.key.repeat))
				{
					pauseMenuLayout.currentlySelected++;
					pauseMenuLayout.currentlySelected = pauseMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
				}

				if (((event.key.keysym.sym == keyboardControls.toggleFullscreen) && !event.key.repeat))
				{
					fullscreen = !fullscreen;
					SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);

					SDL_Log("[%s] Fullscreen Toggled", getTime());
				}

				if (((event.key.keysym.sym == keyboardControls.selectMenuItem) && !event.key.repeat))
				{
					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
					if (pauseMenuLayout.currentlySelected == 0)
					{
						paused = false;
					}
					else if (pauseMenuLayout.currentlySelected == 1)
					{

					}
					else if (pauseMenuLayout.currentlySelected == 2)
					{
						paused = false;
						startMenu = true;
					}
					menuSoundTrigger = true;
				}
				if (((event.key.keysym.sym == keyboardControls.pause) && !event.key.repeat))
				{
					paused = !paused;
					SDL_Log("[%s] Game Unpaused", getTime());
				}

				if (((event.key.keysym.sym == SDLK_m) && !event.key.repeat))
				{
					//If there is no music playing
					if (Mix_PlayingMusic() == 0)
					{
						//Play the music
						Mix_PlayMusic(SOUND_aBckMusic, -1);
						SDL_Log("[%s] Playing Music", getTime());

					}
					else
					{
						Mix_HaltMusic();
						SDL_Log("[%s] Music Paused", getTime());
					}
				}


				break;
			case SDL_JOYBUTTONDOWN:
				if ((event.jbutton.button == 0) || (event.jbutton.button == 7))
				{
					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
					if (pauseMenuLayout.currentlySelected == 0)
					{
						paused = false;
					}
					else if (pauseMenuLayout.currentlySelected == 1)
					{

					}
					else if (pauseMenuLayout.currentlySelected == 2)
					{
						paused = false;
						startMenu = true;
					}
					menuSoundTrigger = true;
				}
				SDL_Log("[%s] Button Pressed", getTime());

				if (event.jbutton.button == 6)
				{
					//If there is no music playing
					if (Mix_PlayingMusic() == 0)
					{
						//Play the music
						Mix_PlayMusic(SOUND_aBckMusic, -1);
						SDL_Log("[%s] Playing Music", getTime());

					}
					else
					{
						Mix_HaltMusic();
						SDL_Log("[%s] Music Paused", getTime());
					}
				}
				break;
			case SDL_JOYHATMOTION:
				if (event.jhat.value == SDL_HAT_RIGHT)
				{
					pauseMenuLayout.currentlySelected--;
					if (pauseMenuLayout.currentlySelected < 0)
					{
						pauseMenuLayout.currentlySelected = 2;
					}
					pauseMenuLayout.currentlySelected = pauseMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
				}
				if (event.jhat.value == SDL_HAT_DOWN)
				{
					pauseMenuLayout.currentlySelected++;
					pauseMenuLayout.currentlySelected = pauseMenuLayout.currentlySelected % 3;

					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
				}
				SDL_Log("[%s]Hat Pressed", getTime());
				break;
			case SDL_MOUSEBUTTONDOWN:
					SDL_Log("[%s] Current Pause Menu Index %i", getTime(), pauseMenuLayout.currentlySelected);
					if (pauseMenuLayout.currentlySelected == 0)
					{
						paused = false;
					}
					else if (pauseMenuLayout.currentlySelected == 1)
					{

					}
					else if (pauseMenuLayout.currentlySelected == 2)
					{
						paused = false;
						startMenu = true;
					}
					menuSoundTrigger = true;
				break;
			default:
				break;
			}
		}
		pauseMenuLayout.Update();
		pauseMenuLayout.InputReceived();
		pauseMenuLayout.Render(aRenderer);
		RunAudio();
		//Shows changes made
		SDL_RenderPresent(aRenderer);
	}
	
}

void GameRunning(SDL_Renderer* aRenderer, SDL_Window* window, bool &running, bool &menu)
{
	int gameLoopTick = SDL_GetTicks();
	int ticksPassed = gameLoopTick - lastGameLoopTick;
	lastGameLoopTick = gameLoopTick;

	SDL_Log("[%s] Game Loop Started: %i Ticks since last Game Loop", getTime(), ticksPassed);

	//Used to calculate is FixedUpdate() should be called
	updateTicks = SDL_GetTicks();

	if (updateTicks > updateLast + updateDelay)
	{
		//Only calls FixedUpdate every (updateDelay) period of time
		FixedUpdate(aRenderer, window, paused, running);
		updateLast = updateTicks;
	}

	if (player1.dead == true)
	{
		menu = true;
	}

	Render(aRenderer, paused);

	//Prevents the rest of the game running while the game is paused
	GamePaused(aRenderer, window, running, 255);
}

void logOverride(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	//Log to console initially
	cout << "\n" << message;
}
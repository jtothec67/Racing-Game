#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>


Game::Game()
{
	Initialise();
}

Game::~Game()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

bool Game::Initialise()
{
	// Initialise everything to do with SDL
	std::cout << "Initialing SDL" << std::endl;
	
	if (!InitSDL())
	{
		return false;
	}
		
	if (!CreateWindow())
	{
		return false;
	}

	if (!CreateRenderer())
	{
		return false;
	}

	if (!InitialiseAudio())
	{
		return false;
	}

	if (!InitialiseTTF())
	{
		return false;
	}

	SetupRenderer();
	
	// Creates white curbs
	int currentY = 1080;
	for (int i = 0; i < 6; i++)
	{
		mLeftWhiteCurbs.push_back(GameObject({ 0,currentY,100,180 }));
		mRightWhiteCurbs.push_back(GameObject({ 700,currentY-180,100,180 }));
		currentY -= 360;
	}

	// Creates a Player
	mPlayer = new Player(300, 600, 200, 350);

	int x = 0;
	int y = 0;

	// Uses players mouse position as seed for random (couldn't get ctime to work)
	SDL_GetMouseState(&x, &y);
	srand(x + y);

	mEnemy1 = new Enemy(100 + rand() % 400, -350, 200, 350);
	mEnemy2 = new Enemy(100 + rand() % 400, -1500, 200, 350);
	mEnemy3 = new Enemy(100 + rand() % 400, -2650, 200, 350);
}

bool Game::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool Game::CreateWindow()
{
	// Window size in a vertical mode because the game suits being played on a phone in my opinion
	mWindow = SDL_CreateWindow("Game", 0, 0, 800, 1080, 0);

	if (mWindow == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}
	return true;
}

bool Game::CreateRenderer()
{
	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

	if (mRenderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}
	return true;
}

void Game::SetupRenderer()
{
	SDL_RenderSetLogicalSize(mRenderer, 800, 1080);

	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
}

bool Game::InitialiseTTF()
{
	if (TTF_Init() < 0)
	{
		std::cout << "Failed to initialise TTF : " << SDL_GetError();
		return false;
	}
	return true;
}

void Game::Run()
{
	// Load all textures before the game loop to later be passed into Renderer()
	const char* image_path = "playerCar.bmp";
	SDL_Surface* image = SDL_LoadBMP(image_path);
	if (!image)
	{
		std::cout << "Failed to load image : " << SDL_GetError();
	}
	SDL_Texture* playerCar{ nullptr };
	playerCar = SDL_CreateTextureFromSurface(mRenderer, image);

	const char* image_path2 = "enemyCar1.bmp";
	SDL_Surface* image2 = SDL_LoadBMP(image_path2);
	if (!image2)
	{
		std::cout << "Failed to load image : " << SDL_GetError();
	}
	SDL_Texture* enemyCar1{ nullptr };
	enemyCar1 = SDL_CreateTextureFromSurface(mRenderer, image2);

	const char* image_path3 = "enemyCar2.bmp";
	SDL_Surface* image3 = SDL_LoadBMP(image_path3);
	if (!image3)
	{
		std::cout << "Failed to load image : " << SDL_GetError();
	}
	SDL_Texture* enemyCar2{ nullptr };
	enemyCar2 = SDL_CreateTextureFromSurface(mRenderer, image3);

	const char* image_path4 = "enemyCar3.bmp";
	SDL_Surface* image4 = SDL_LoadBMP(image_path4);
	if (!image4)
	{
		std::cout << "Failed to load image : " << SDL_GetError();
	}
	SDL_Texture* enemyCar3{ nullptr };
	enemyCar3 = SDL_CreateTextureFromSurface(mRenderer, image4);

	

	TTF_Font* font{ nullptr };
	font = TTF_OpenFont("font.ttf", 24);
	if (!font)
	{
		std::cout << "Failed to load font : " << SDL_GetError();
	}

	bool gameEnded = false;
	bool mouse = false;
	bool menu = true;
	int score = 0;
	// GetHighestScore() reads the value in highScore.txt and returns it
	int highestScore = GetHighestScore();

	// Game loop
	while (!gameEnded)
	{
		if (!menu)
		{
			// Check for player input
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					gameEnded = true;
				}
				else
				{
					if (event.type == SDL_KEYDOWN)
					{
						// Moves player if player input has been detected
						mouse = false;
						mPlayer->Move(event, mouse);
					}
					else if (event.type == SDL_MOUSEMOTION)
					{
						mouse = true;
						mPlayer->Move(event, mouse);
					}
				}
			}
			// Update() updates enemy positions and detects if a crash has happened and user needs to be returned to the menu,
			// also returns score to be displayed on the menu if there is a crash
			score = Update(menu);
		}
		else
		{
			if (score > highestScore)
			{
				// Updates the highScore.txt file with the new high score
				highestScore = score;
				UpdateHighScore(highestScore);
			}
			// Runs/renders/handels user input on the menu screen
			Menu(font, menu, score, highestScore);
		}
		// Renders the game
		Render(playerCar, enemyCar1, enemyCar2, enemyCar3, menu, font);
		// 60FPS
		SDL_Delay(16);
	}
}

int Game::Update(bool& menu)
{
	int score = 0;

	// Increases the enemies speed
	mEnemy1->setSpeed(mEnemy1->getSpeed() + 1);
	mEnemy2->setSpeed(mEnemy2->getSpeed() + 1);
	mEnemy3->setSpeed(mEnemy3->getSpeed() + 1);

	// Moves the enemies down the screen/teleports them back up above the screen if they reach the bottom
	mEnemy1->Move();
	mEnemy2->Move();
	mEnemy3->Move();

	// Moves white curbs down the screen/teleports them back up above the screen if they reach the bottom
	for (int i = 0; i < 6; ++i)
	{
		if (mLeftWhiteCurbs[i].getPosY() >= 1080)
		{
			mLeftWhiteCurbs[i].setPosY(mLeftWhiteCurbs[i].getPosY() - 1440);
		}
		else
		{
			mLeftWhiteCurbs[i].setPosY(mLeftWhiteCurbs[i].getPosY() + (mEnemy1->getSpeed() / 75));
		}
		

		if (mRightWhiteCurbs[i].getPosY() >= 1080)
		{
			mRightWhiteCurbs[i].setPosY(mRightWhiteCurbs[i].getPosY() - 1440);
		}
		else
		{
			mRightWhiteCurbs[i].setPosY(mRightWhiteCurbs[i].getPosY() + (mEnemy1->getSpeed() / 75));
		}
		
	}

	if (CheckForCollisions()) // If a crash
	{
		// Crash sound plays
		srand(time(0));
		int randAudio = rand() % mCrashAudios.size();
		Mix_PlayChannel(-1, mCrashAudios[randAudio], 0);

		// Updating score to be shown on the menu
		score = (mEnemy1->getSpeed() - 500) / 10;

		// Will reset enemies positions/speed
		Reset();

		// Menu passed by reference so will change to true back in the game loop
		menu = true;
	}

	return score;
}

void Game::Render(SDL_Texture *playerCar, SDL_Texture *enemyCar1, SDL_Texture* enemyCar2, SDL_Texture* enemyCar3, bool& menu, TTF_Font* font)
{
	if (menu) return; // If menu is true, don't bother rendering the rest of the game


	// Clears screen in grey colour which will be the road
	SDL_SetRenderDrawColor(mRenderer, 213, 214, 210, 255);
	SDL_RenderClear(mRenderer);

	// Renders the side bars in red
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 30, 255);
	SDL_RenderFillRect(mRenderer, &mLeftBar);
	SDL_RenderFillRect(mRenderer, &mRightBar);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	for (int i = 0; i < 6; ++i)
	{
		SDL_Rect leftCurb = mLeftWhiteCurbs[i].getRect();
		SDL_RenderFillRect(mRenderer, &leftCurb);

		SDL_Rect rightCurb = mRightWhiteCurbs[i].getRect();
		SDL_RenderFillRect(mRenderer, &rightCurb);
	}

	// Renders the player and enemies
	SDL_Rect dest{ mPlayer->getPosX(),mPlayer->getPosY(),200,350 };
	SDL_RenderCopy(mRenderer, playerCar, NULL, &dest);

	SDL_Rect dest2{ mEnemy1->getPosX(),mEnemy1->getPosY(),200,350 };
	SDL_RenderCopy(mRenderer, enemyCar1, NULL, &dest2);

	SDL_Rect dest3{ mEnemy2->getPosX(),mEnemy2->getPosY(),200,350 };
	SDL_RenderCopy(mRenderer, enemyCar2, NULL, &dest3);

	SDL_Rect dest4{ mEnemy3->getPosX(),mEnemy3->getPosY(),200,350 };
	SDL_RenderCopy(mRenderer, enemyCar3, NULL, &dest4);

	SDL_Surface* text{ nullptr };
	SDL_Color color = { 0, 0, 0 };

	// Renders the score
	std::stringstream string;
	string << (mEnemy1->getSpeed() - 500) / 10; // Speed - 500 because it starts at 500 so score will start at 0, / 10 to make the number smaller and more readable
		
	// Converting string to char const*
	std::string temp_str = string.str();
	char const* score = temp_str.c_str();

	// Renders the texts surface
	text = TTF_RenderText_Solid(font, score, color);

	// Creating a texture from the surface and rendering it on to the screen
	SDL_Texture* text_texture;
	text_texture = SDL_CreateTextureFromSurface(mRenderer, text);
	SDL_Rect dest5 = { 110, 10, text->w * 5, text->h * 5 };
	SDL_RenderCopy(mRenderer, text_texture, NULL, &dest5);

	SDL_RenderPresent(mRenderer);

	// Free up memory
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text);
}

bool Game::CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	// Checks for a collision bewteen the two rects
	if (rect1.x > rect2.x + rect2.w)
	{
		return false;
	}
	if (rect1.x + rect1.w < rect2.x)
	{
		return false;
	}
	if (rect1.y > rect2.y + rect2.h)
	{
		return false;
	}
	if (rect1.y + rect1.h < rect2.y)
	{
		return false;
	}

	return true;
}

bool Game::CheckForCollisions()
{
	// Checks for collisions of every enemy and the player
	if (!CheckCollision(mEnemy1->getRect(), mPlayer->getRect()) &&
		!CheckCollision(mEnemy2->getRect(), mPlayer->getRect()) &&
		!CheckCollision(mEnemy3->getRect(), mPlayer->getRect()))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Game::Reset()
{
	// Resets the enemy and players x positions
	mEnemy1->setPosX(100 + rand() % 400);
	mEnemy2->setPosX(100 + rand() % 400);
	mEnemy3->setPosX(100 + rand() % 400);
	mPlayer->setPosX(300);

	// Resets the enemy and players x positions
	mEnemy1->setPosY(-350);
	mEnemy2->setPosY(-1500);
	mEnemy3->setPosY(-2650);
	mPlayer->setPosY(500);

	// Resets the enemies speed
	mEnemy1->setSpeed(500);
	mEnemy2->setSpeed(500);
	mEnemy3->setSpeed(500);
}

bool Game::InitialiseAudio()
{
	int audio = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	if (audio != 0)
	{
		std::cout << " Failed to initialize audio : " << SDL_GetError() << std::endl;
		return false;
	}

	Mix_Music* theChain{ nullptr };
	theChain = Mix_LoadMUS("theChainLoop.WAV");
	if (!theChain)
	{
		std::cout << "Failed to load music : " << SDL_GetError();
		return false;
	}
	Mix_VolumeMusic(20);
	Mix_PlayMusic(theChain, -1);


	Mix_Volume(0, 64);
	Mix_Chunk* crash{ nullptr };
	crash = Mix_LoadWAV("oohThatsABigCrash.mp3");
	if (!crash)
	{
		std::cout << "Failed to load crash sound : " << SDL_GetError();
		return false;
	}
	mCrashAudios.push_back(crash);

	Mix_Chunk* crash2{ nullptr };
	crash2 = Mix_LoadWAV("FernandoOverTheTop.mp3");
	if (!crash2)
	{
		std::cout << "Failed to load crash sound 2 : " << SDL_GetError();
		return false;
	}
	mCrashAudios.push_back(crash2);

	Mix_Chunk* crash3{ nullptr };
	crash3 = Mix_LoadWAV("bigBigAccident.mp3");
	if (!crash3)
	{
		std::cout << "Failed to load crash sound 3 : " << SDL_GetError();
		return false;
	}
	mCrashAudios.push_back(crash3);

	Mix_Chunk* crash4{ nullptr };
	crash4 = Mix_LoadWAV("OHNO.mp3");
	if (!crash4)
	{
		std::cout << "Failed to load crash sound 4 : " << SDL_GetError();
		return false;
	}
	mCrashAudios.push_back(crash4);

	Mix_Chunk* crash5{ nullptr };
	crash5 = Mix_LoadWAV("touchedCollided.mp3");
	if (!crash5)
	{
		std::cout << "Failed to load crash sound 5 : " << SDL_GetError();
		return false;
	}
	mCrashAudios.push_back(crash5);

	return true;
}

void Game::Menu(TTF_Font* font, bool& menu, int score, int highestScore)
{
	// Creates a white background
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	if (score > 0) // Score will only be 0 if it's the first time playing the game after running
	{
		// This code displays the last score you got and the high score on the menu
		SDL_Surface* text1{ nullptr };
		SDL_Color color1 = { 0, 0, 0 };

		std::stringstream string;
		string << score;
		std::string temp_str = string.str();
		char const* score = temp_str.c_str();

		std::string startOfMessage = "Your score was ";
		std::string endOfMessage = "!";
		std::string message = startOfMessage + score + endOfMessage;
		const char* output = message.c_str();

		text1 = TTF_RenderText_Solid(font, output, color1);

		SDL_Texture* text_texture1;
		text_texture1 = SDL_CreateTextureFromSurface(mRenderer, text1);
		SDL_Rect dest5 = { 90, 500, text1->w * 3, text1->h * 3 };
		SDL_RenderCopy(mRenderer, text_texture1, NULL, &dest5);

		SDL_DestroyTexture(text_texture1);

		SDL_FreeSurface(text1);


		SDL_Surface* text2{ nullptr };
		SDL_Color color2 = { 0, 0, 0 };

		std::stringstream string2;
		string2 << highestScore;
		std::string temp_str2 = string2.str();
		char const* score2 = temp_str2.c_str();

		std::string startOfMessage2 = "The high score is ";
		std::string endOfMessage2 = "!";
		std::string message2 = startOfMessage2 + score2 + endOfMessage2;
		const char* output2 = message2.c_str();

		text2 = TTF_RenderText_Solid(font, output2, color2);

		SDL_Texture* text_texture2;
		text_texture2 = SDL_CreateTextureFromSurface(mRenderer, text2);
		SDL_Rect dest6 = { 50, 700, text2->w * 2.5, text2->h * 2.5 };
		SDL_RenderCopy(mRenderer, text_texture2, NULL, &dest6);

		SDL_DestroyTexture(text_texture2);

		SDL_FreeSurface(text2);
	}

	// Creating the start button
	SDL_Rect startButton = { 250, 200, 300, 150 };
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(mRenderer, &startButton);

	// Creating the "Start" text
	SDL_Surface* text{ nullptr };
	SDL_Color color = { 0, 0, 0 };

	text = TTF_RenderText_Solid(font, "Start", color);

	SDL_Texture* text_texture;
	text_texture = SDL_CreateTextureFromSurface(mRenderer, text);
	SDL_Rect dest = { 250, 225, text->w*5, text->h*5 };
	SDL_RenderCopy(mRenderer, text_texture, NULL, &dest);

	SDL_RenderPresent(mRenderer);

	SDL_DestroyTexture(text_texture);

	SDL_FreeSurface(text);

	if (CheckStartClicked()) // Checks to see if the user has clicked on the start button
	{
		menu = false; // menu passed by reference so will be altered back in the game loop
	}
}

bool Game::CheckStartClicked()
{
	SDL_Event event;

	int x = 0;
	int y = 0;

	// Gets users mouse position
	SDL_GetMouseState(&x, &y);

	while (SDL_PollEvent(&event))
	{
		if (event.button.button == SDL_BUTTON_LEFT && // Checks if user has left clicked the mouse while it is over the start button
			x >= 250 &&
			x <= 550 &&
			y >= 200 &&
			y <= 350)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void Game::UpdateHighScore(int newScore)
{
	// Converts the highScore int to a string
	std::string value = std::to_string(newScore);
	
	// Opens the highScore.txt file
	std::fstream highScoreFile;
	highScoreFile.open("highScore.txt", std::ios::out);
	if (highScoreFile.is_open())
	{
		for (int i = 0; i < value.size(); ++i) { // Writes the new high score over the old high score
			highScoreFile << value[i];
		}
	}
	highScoreFile.close();
}

int Game::GetHighestScore()
{
	// Opens the highScore.txt file
	std::fstream highScoreFile;
	highScoreFile.open("highScore.txt", std::ios::in);
	std::string fileValue("");

	if (highScoreFile.is_open())
	{
		while (!highScoreFile.eof()) // Reads evrything in highScore.txt and stores it in fileValue
		{
			char c{ 0 };
			highScoreFile >> c;
			fileValue += c;
		}
	}
	highScoreFile.close();

	// fileValue converted from a string to an int and returned
	int value = std::stoi(fileValue);
	return value;
}
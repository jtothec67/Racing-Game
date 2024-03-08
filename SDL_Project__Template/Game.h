#define _GAME_H_
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <SDL_mixer.h>


class Game
{
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Rect mRightBar{ 700,0,100,1080 }; // Predefined positions of the side bars
	SDL_Rect mLeftBar{ 0,0,100,1080 };
	std::vector<GameObject> mLeftWhiteCurbs;
	std::vector<GameObject> mRightWhiteCurbs;
	Player* mPlayer = nullptr;
	Enemy* mEnemy1 = nullptr;
	Enemy* mEnemy2 = nullptr;
	Enemy* mEnemy3 = nullptr;
	std::vector<Mix_Chunk*> mCrashAudios;
public:
	Game();
	~Game();
	void Run();
private:
	bool Initialise();
	bool InitSDL();
	bool CreateWindow();
	bool CreateRenderer();
	void SetupRenderer();
	int Update(bool& menu);
	void Render(SDL_Texture *playerCar, SDL_Texture* enemyCar1, SDL_Texture* enemyCar2, SDL_Texture* enemyCar3, bool& menu, TTF_Font* font);
	bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);
	bool CheckForCollisions();
	void Reset();
	bool InitialiseAudio();
	void Menu(TTF_Font* font, bool& menu, int score, int highestScore);
	bool InitialiseTTF();
	bool CheckStartClicked();
	void UpdateHighScore(int highestScore);
	int GetHighestScore();
};
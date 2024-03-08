#include "Player.h"
#include <SDL.h>

Player::Player()
{

}

Player::Player(int _x, int _y, int _w, int _h)
{
	mPos.x = _x; mPos.y = _y;
	mPos.w = _w; mPos.h = _h;
}

Player::~Player()
{
}

void Player::Move(SDL_Event& event, bool mouse)
{
	int x = 0;
	int y = 0;

	// Gets players mouse position
	SDL_GetMouseState(&x, &y);

	if (!mouse)
	{
		//keys is the keyboard state
		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_D] && keys[SDL_SCANCODE_W]) // These ifs allow for diagonal movement to help make the game flow more
		{
			mPos.x += movementFactor / 2; // / 2 so that you can still only move the desired movement factor in one frame
			mPos.y -= movementFactor / 2;
		}
		else if (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_W])
		{
			mPos.x -= movementFactor / 2;
			mPos.y -= movementFactor / 2;
		}
		else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
		{
			mPos.y += movementFactor / 2;
			mPos.x += movementFactor / 2;
		}
		else if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A])
		{
			mPos.y -= movementFactor / 2;
			mPos.x -= movementFactor / 2;
		}
		else if (keys[SDL_SCANCODE_D]) // Gets the keyboard state and moves the player accordingly
		{
			mPos.x += movementFactor;
		}
		else if (keys[SDL_SCANCODE_A])
		{
			mPos.x -= movementFactor;
		}
		else if (keys[SDL_SCANCODE_S])
		{
			mPos.y += movementFactor;
		}
		else if (keys[SDL_SCANCODE_W])
		{
			mPos.y -= movementFactor;
		}
	}
	else if (event.button.button == SDL_BUTTON_LEFT && // Will only move the player if they are currently left clicking on the player
		x >= mPos.x &&
		x <= mPos.x+mPos.w &&
		y >= mPos.y &&
		y <= mPos.y+mPos.h)
	{
		mPos.x = x - mPos.w / 2; // / 2 makes the mouse central in the players rect
		mPos.y = y - mPos.h / 2;
	}

	// Makes sure the player can't move out of bounds
	if (mPos.x >= 500)
	{
		mPos.x = 500;
	}
	if (mPos.x < 100)
	{
		mPos.x = 100;
	}
	if (mPos.y > 730)
	{
		mPos.y = 730;
	}
	if (mPos.y < 0)
	{
		mPos.y = 0;
	}
}
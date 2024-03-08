#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include <SDL.h>


class Player : public GameObject
{
public:
	int movementFactor = 70; // Used to move the player this amount of pixels
	Player();
	Player(int _x, int _y, int _w, int _h);
	~Player();
	void Move(SDL_Event& event, bool mouse);
};

#endif
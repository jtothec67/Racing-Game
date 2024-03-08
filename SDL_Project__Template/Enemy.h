#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "GameObject.h"
#include <SDL.h>

class Enemy : public GameObject
{
private:
	int mSpeed = 500; // How many pixels the enemies move each frame, start at 500 instead of 0 so game doesn't start too slowly. Will be / 100 so speed increases every 20 frames
public:
	Enemy();
	Enemy(int _x, int _y, int _w, int _h);
	~Enemy();
	void setSpeed(int _speed);
	int getSpeed(void);
	void Move();
	void ResetEnemyPos();
};
#endif
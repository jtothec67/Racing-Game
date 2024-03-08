#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
{
}

Enemy::Enemy(int _x, int _y, int _w, int _h)
{
	mPos.x = _x; mPos.y = _y;
	mPos.w = _w; mPos.h = _h;
}

Enemy::~Enemy()
{
}

void Enemy::setSpeed(int _speed) { mSpeed = _speed; }

int Enemy::getSpeed(void) { return mSpeed; }

void Enemy::Move()
{
	// Moves enemies
	this->setPosY(this->getPosY() + (this->getSpeed()/100));
	
	// Resets their position if they below the bottom of the screen
	if (this->getPosY() >= 1080) 
	{
		ResetEnemyPos();
	}
}

void Enemy::ResetEnemyPos()
{
	// Resets the enemy above the screen so by the time they will get down into the screen they are a normal distance from the last car
	this->setPosY(-2440);
	
	// Gives them a random x value
	this->setPosX(100 + rand() % 400);
}
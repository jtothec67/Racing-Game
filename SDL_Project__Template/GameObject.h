#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#include <SDL.h>

class GameObject
{
protected:
	SDL_Rect mPos; // Only attribute all game objects will share
public:
	GameObject();
	GameObject(SDL_Rect _pos);
	~GameObject();
	void setPosX(int _x);
	int getPosX(void);
	void setPosY(int _x);
	int getPosY(void);
	void setW(int _w);
	int getW(void);
	void setH(int _h);
	int getH(void);
	SDL_Rect getRect();
	void setRect(SDL_Rect _rect);
};
#endif
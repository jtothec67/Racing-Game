#include "GameObject.h"

GameObject::GameObject()
{
}
GameObject::GameObject(SDL_Rect _pos)
{
	mPos = _pos;
}
GameObject::~GameObject()
{
}

void GameObject::setPosX(int _x) { mPos.x = _x; }

int GameObject::getPosX(void) { return mPos.x; }

void GameObject::setPosY(int _y) { mPos.y = _y; }

int GameObject::getPosY(void) { return mPos.y; }

void GameObject::setW(int _w) { mPos.w = _w; }

int GameObject::getW(void) { return mPos.w; }

void GameObject::setH(int _h) { mPos.h = _h; }

int GameObject::getH(void) { return mPos.h; }

SDL_Rect GameObject::getRect() { return mPos; }

void GameObject::setRect(SDL_Rect _rect) { mPos = _rect; }


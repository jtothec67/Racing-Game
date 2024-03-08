#include "Game.h"
#include <iostream>

int main(int argc, char* argv[])
{
	Game* gptr = nullptr;
	gptr = new Game;
	if (gptr != nullptr)
	{
		std::cout << "Game Created" << std::endl;
		gptr->Run();
		delete gptr;
		gptr = nullptr;
	}
	else
	{
		std::cout << "error creating game object" << std::endl;
	}
	return 0;
}
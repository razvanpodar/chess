#include <iostream>
#include "SDL.h"
#include "Chess.h"

#define DSDL_MAIN_HANDLED

const int WIDTH = 640;
const int HEIGHT = 640;

// TODO: Implement delta time

int main(int argc, char* argv[])
{
	Chess chess(WIDTH, HEIGHT);

	while (chess.IsRunning())
	{
		chess.Update();
	}

	return 0;
}
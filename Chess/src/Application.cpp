#include <iostream>
#include "SDL.h"
#include "Chess.h"

// to be deleted
#include "Board.h";

#define DSDL_MAIN_HANDLED

const int WIDTH = 640;
const int HEIGHT = 640;

// TODO: Implement delta time

int main(int argc, char* argv[])
{
	Chess chess(WIDTH, HEIGHT);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);

	while (chess.IsRunning())
	{
		chess.Update();
	}

	return 0;
}
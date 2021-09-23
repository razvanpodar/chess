#include <iostream>
#include "SDL.h"


// to be deleted
#include "Board.h";

#define DSDL_MAIN_HANDLED

const int WIDTH = 960;
const int HEIGHT = 960;

// TODO: Implement delta time

struct Color {
	int R, G, B;
};

void drawTileMap(SDL_Renderer* renderer, int screenWidth, int screenHeight, int tiles)
{
	Color colors[2];
	colors[0] = { 69, 190, 100 };
	colors[1] = { 100, 25, 150 };
	
	int tileWidth = screenWidth / tiles;
	int tileHeight = screenHeight / tiles;

	for (int i = 0; i < tiles; i++)
	{
		for (int j = 0; j < tiles; j++)
		{
			int c = (i + j) % 2;
			SDL_SetRenderDrawColor(renderer, colors[c].R, colors[c].G, colors[c].B, 0);
			SDL_Rect rect;
			rect.x = i * tileWidth;
			rect.y = j * tileHeight;
			rect.w = tileWidth;
			rect.h = tileHeight;

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void dragAndDrop(int posX, int poxY, int tileWidth, int tileHeight, bool isDragged)
{
	
}

void drawPiece(SDL_Renderer* renderer, int posX, int posY, int tileWidth, int tileHeight)
{
	if (posX > 7 || posY > 7)
	{
		printf("Invalid possition");
	}
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
	int rectSize = 40;

	SDL_Rect rect;
	rect.x = (posX * tileWidth) + (rectSize / 4);
	rect.y = (posY * tileHeight) + (rectSize / 4);
	rect.w = rectSize;
	rect.h = rectSize;

	SDL_RenderFillRect(renderer, &rect);
}

void drawPieceByCoords(SDL_Renderer* renderer, int posX, int posY)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
	int rectSize = 40;

	SDL_Rect rect;
	rect.x = posX - (rectSize / 2);
	rect.y = posY - (rectSize / 2);
	rect.w = rectSize;
	rect.h = rectSize;

	SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

	if (window == NULL)
	{
		std::cout << "Failed to create window!" << std::endl;
		return 1;
	}

	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	Board board(renderer, WIDTH, HEIGHT);

	SDL_Event event;
	bool running = true;
	int x = 0, y = 0;
	int pointX = 4, pointY = 7;
	int lastClickX = 0, lastClickY = 0;
	bool isDragged = false;
	bool isClicked = false;
	Piece draggedPiece(-1, Type::Empty);
	while(running) 
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				SDL_Log("Program quit after %i ticks", event.quit.timestamp);
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					SDL_GetMouseState(&x, &y);
					printf("Begin drag: %d, %d\n", x, y);
					draggedPiece = board.GetPiece(x, y);
					isDragged = true;
					lastClickX = x;
					lastClickY = y;
					printf("%d, %d\n", lastClickX, lastClickY);
				default:
					break;
				}
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					if (event.button.state == SDL_RELEASED)
					{
						SDL_GetMouseState(&x, &y);
						printf("End drag: %d, %d\n", x, y);
						isDragged = false;
					}
				default:
					break;
				}
			default:
				break;
			}
		}
		
		//SDL_RenderClear(renderer);
		//printf("%d\n", isDragged);
		//if (isClicked)
		//{
		//	printf("%d\n", isClicked);
		//	isClicked = false;
		//}

		//drawTileMap(renderer, WIDTH, HEIGHT, 8);

		//printf("%d, %d\n", lastClickX, lastClickY);
		board.Update(renderer, isDragged, x, y, draggedPiece, lastClickX, lastClickY);

		//drawPiece(renderer, pointX, pointY, 60, 60);
		//if (!isDragged) {
		//	drawPiece(renderer, x / 60, y / 60, 60, 60);
		//} 
		//else
		//{
		//	SDL_GetMouseState(&x, &y);
		//	drawPieceByCoords(renderer, x, y);
		//}

		// SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
#pragma once
#include "SDL.h"
#include "Board.h"

class Chess
{
private:
	bool m_IsRunning = true;

public:
	SDL_Window *m_Window;
	int m_WindowWidth = 0;
	int m_WindowHeight = 0;
	Board m_Board;
	
	SDL_Event m_Event;
	int x = 0, y = 0;
	int pointX = 4, pointY = 7;
	int lastClickX = 0, lastClickY = 0;
	bool isDragged = false;
	bool isClicked = false;
	Piece draggedPiece;

public:
	Chess(int width, int height);
	~Chess();

	void Init();
	void Update();
	void Quit() { m_IsRunning = false; }
	bool IsRunning() { return m_IsRunning; }
};
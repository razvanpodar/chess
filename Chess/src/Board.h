#pragma once
#include "SDL.h"
#include "Piece.h"

struct PixelColor {
	int R, G, B;
};

class Board
{
public:
	// The type will be replaced with the class Piece
	Piece m_Board[8][8];
	const int m_Size = 8;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_TileWidth;
	int m_TileHeight;
	PixelColor m_BoardColor[2];

	// This will be replaced with the texture of each piece
	PixelColor m_PixelColor[8];

public:
	Board(int screenWidth, int screenHeight);
	~Board();

	void Update(SDL_Renderer *renderer, bool isDragged, int mousePosX, 
		int mousePosY, Piece draggedPiece);
	void UpdatePiece(SDL_Renderer* renderer, int mousePosX, int mousePosY, 
		Piece draggedPiece);
	void UpdatePieceByCoordinates(SDL_Renderer* renderer, int mousePosX, 
		int mousePosY, Piece draggedPiece);
	Piece GetPiece(int mousePosX, int mousePosY);
private:
	void Init();
	void DrawBoard(SDL_Renderer *renderer);
	void DrawPieces(SDL_Renderer* renderer);
};
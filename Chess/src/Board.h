#pragma once

#include <string>
#include "SDL.h"
#include "Piece.h"

struct PixelColor {
	int R, G, B;
};

const std::string files[12] = { "white_pawn.png", "white_rook.png", 
	"white_knight.png", "white_bishop.png", "white_queen.png", "white_king.png", 
	"black_pawn.png", "black_rook.png", "black_knight.png", "black_bishop.png", 
	"black_queen.png", "black_king.png" };
const std::string path = "resources/pieces/";

class Board
{
public:
	// The type will be replaced with the class Piece
	Piece m_Board[8][8];
	// For the empty spaces set to null 
	// Or set type to Empty and check the type when rendering

	const int m_Size = 8;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_TileWidth;
	int m_TileHeight;
	PixelColor m_BoardColor[2];

	// This will be replaced with the texture of each piece
	PixelColor m_PixelColor[8];

public:
	Board(SDL_Renderer *renderer, int screenWidth, int screenHeight);
	~Board();

	void Update(SDL_Renderer *renderer, bool isDragged, int mousePosX, 
		int mousePosY, Piece draggedPiece, int lastClickX, int lastClickY);
	void UpdatePiece(SDL_Renderer* renderer, int mousePosX, int mousePosY, 
		Piece draggedPiece, int lastClickX, int lastClickY);
	void UpdatePieceByCoordinates(SDL_Renderer* renderer, int mousePosX, 
		int mousePosY, Piece draggedPiece, int lastClickX, int lastClickY);
	Piece GetPiece(int mousePosX, int mousePosY);
private:
	void Init(SDL_Renderer *renderer);
	void DrawBoard(SDL_Renderer *renderer);
	void DrawPieces(SDL_Renderer* renderer);
};
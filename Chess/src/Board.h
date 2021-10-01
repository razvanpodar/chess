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
	Piece m_Board[8][8] = { Piece(-1, Type::Empty) };
	int m_Moves[8][8] = { 0 };
	// For the empty spaces set to null 
	// Or set type to Empty and check the type when rendering

	const int m_Size = 8;
	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	int m_TileWidth = 0;
	int m_TileHeight = 0;
	PixelColor m_BoardColor[2];
	SDL_Renderer *m_Renderer;

	bool m_Turn = 1;

public:
	Board();
	Board(int screenWidth, int screenHeight);
	~Board();

	void Init(SDL_Window *window);
	void Reset();
	void Update(bool isDragged, int mousePosX, int mousePosY, Piece draggedPiece, 
		int lastClickX, int lastClickY);
	void UpdatePiece(int mousePosX, int mousePosY, Piece draggedPiece, 
		int lastClickX, int lastClickY);
	void UpdatePieceByCoordinates(int mousePosX, int mousePosY, Piece draggedPiece, 
		int lastClickX, int lastClickY);
	Piece GetPiece(int mousePosX, int mousePosY);
	void SetScreenWidthAndHeight(int screenWidth, int screenHeight)
	{
		m_ScreenWidth = screenWidth;
		m_ScreenHeight = screenHeight;
	}
private:
	void DrawBoard();
	void DrawPieces();
	void DrawMoves();
	void FindLegalMoves(int x, int y, Piece piece);
	void ResetLegalMoves();
	bool LegalSquare(int x, int y, Piece piece);

	void PawnLegalMoves(int x, int y, Piece piece);
	void RookLegalMoves(int x, int y, Piece piece);
	void KnightLegalMoves(int x, int y, Piece piece);
	void BishopLegalMoves(int x, int y, Piece piece);
	void QueenLegalMoves(int x, int y, Piece piece);
	void KingLegalMoves(int x, int y, Piece piece);
};
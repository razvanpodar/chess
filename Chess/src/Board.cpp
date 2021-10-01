#include "Board.h"
#include "TextureManager.h"


// Create and 2D array with the initial board state
// Piece InitialBoard[8][8] = { Piece(-1, Type::Empty) };
int InitialBoard[8][8] = { -2, -3, -4, -5, -6, -4, -3, -2,
						   -1, -1, -1, -1, -1, -1, -1, -1,
						    0,  0,  0,  0,  0,  0,  0,  0,
						    0,  0,  0,  0,  0,  0,  0,  0,
						    0,  0,  0,  0,  0,  0,  0,  0,
						    0,  0,  0,  0,  0,  0,  0,  0,
						    1,  1,  1,  1,  1,  1,  1,  1,
						    2,  3,  4,  5,  6,  4,  3,  2 };

Board::Board()
{ }

Board::Board(int screenWidth, int screenHeight)
{
	//m_BoardColor[0] = { 69, 190, 100 };
	//m_BoardColor[1] = { 100, 25, 150 };

	m_BoardColor[0] = { 220, 210, 150 };
	m_BoardColor[1] = { 120,  80,  10 };

	m_TileWidth = m_ScreenWidth / m_Size;
	m_TileHeight = m_ScreenHeight / m_Size;
}

Board::~Board()
{
	SDL_DestroyRenderer(m_Renderer);
}

void Board::Init(SDL_Window *window)
{
	//m_BoardColor[0] = { 69, 190, 100 };
	//m_BoardColor[1] = { 100, 25, 150 };

	m_BoardColor[0] = { 220, 210, 150 };
	m_BoardColor[1] = { 120,  80,  10 };

	m_TileWidth = m_ScreenWidth / m_Size;
	m_TileHeight = m_ScreenHeight / m_Size;

	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Reset();
}

void Board::Reset()
{
	ResetLegalMoves();

	float padPercent = 0.1f;
	int pad = m_TileWidth * padPercent;
	int rectWidth = m_TileWidth - (2 * pad);
	int rectHeight = m_TileHeight - (2 * pad);

	TextureManager texMan;
	// Initialize from an array
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece p(-1, Type::Empty);
			int index = 0;
			if (InitialBoard[i][j] < 0)
				p.SetSide(1);
			if (InitialBoard[i][j] > 0)
				p.SetSide(0);

			index = (p.GetSide() * 6) + std::abs(InitialBoard[i][j]) - 1;
			p.SetType(static_cast<Type>(std::abs(InitialBoard[i][j])));
			if (InitialBoard[i][j] != 0)
				p.SetTexture(texMan.LoadTexture(m_Renderer, (path + files[index]).c_str()));

			m_Board[j][i] = p;
		}
	}
}

void Board::Update(bool isDragged, int mousePosX, 
	int mousePosY, Piece draggedPiece, int lastClickX, int lastClickY)
{
	// Draw board
	DrawBoard();
	DrawMoves();
	DrawPieces();

	if (!isDragged) {
		UpdatePiece(mousePosX, mousePosY, draggedPiece, lastClickX, lastClickY);
	} 
	else
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		UpdatePieceByCoordinates(x, y, draggedPiece, lastClickX, lastClickY);
	}

	// Make move (logic)
	SDL_RenderPresent(m_Renderer);
}

void Board::DrawBoard()
{
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			int c = (i + j) % 2;
			SDL_SetRenderDrawColor(m_Renderer, m_BoardColor[c].R, m_BoardColor[c].G, 
				m_BoardColor[c].B, 0);
			SDL_Rect rect;
			rect.x = i * m_TileWidth;
			rect.y = j * m_TileHeight;
			rect.w = m_TileWidth;
			rect.h = m_TileHeight;

			SDL_RenderFillRect(m_Renderer, &rect);
		}
	}
}

void Board::DrawPieces()
{
	float padPercent = 0.1f;
	int pad = m_TileWidth * padPercent;
	int rectWidth = m_TileWidth - (2 * pad);
	int rectHeight = m_TileHeight - (2 * pad);

	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece piece = m_Board[i][j];

			if (piece.GetType() == Type::Empty) continue;

			SDL_Rect rect;
			rect.x = (i * m_TileWidth) + pad;
			rect.y = (j * m_TileHeight) + pad;
			rect.w = rectWidth;
			rect.h = rectHeight;

			SDL_RenderCopy(m_Renderer, piece.GetTexture(), NULL, &rect);
		}
	}
}

void Board::DrawMoves()
{
	PixelColor rectColor[2];
	//rectColor[0] = {   0, 255, 0 };
	//rectColor[1] = { 255,   0, 0 };

	rectColor[0] = { 125, 255, 100 };
	rectColor[1] = { 250,  90,  70 };

	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			if (m_Moves[i][j] != 0)
			{
				int v = m_Moves[i][j] - 1;
				SDL_SetRenderDrawColor(m_Renderer, rectColor[v].R, rectColor[v].G, 
					rectColor[v].B, 0);

				SDL_Rect rect;
				rect.x = i * m_TileWidth;
				rect.y = j * m_TileHeight;
				rect.w = m_TileWidth;
				rect.h = m_TileHeight;

				SDL_RenderFillRect(m_Renderer, &rect);

				SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
				SDL_RenderDrawRect(m_Renderer, &rect);
			}
		}
	}
}

void Board::UpdatePiece(int mousePosX, int mousePosY, Piece draggedPiece, 
	int lastClickX, int lastClickY)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	int lastPosX = lastClickX / m_TileWidth;
	int lastPosY = lastClickY / m_TileHeight;

	// Check for invalid position
	if (posX < 0 || posX > 7 || posY < 0 || posY > 7 || lastPosX < 0 
		|| lastPosX > 7 || lastPosY < 0 || lastPosY > 7) return;
	// Check if the location is a valid move

	if (draggedPiece.GetType() != Type::Empty)
	{
		if (m_Turn == draggedPiece.GetSide()) return;
		if ((posX != lastPosX) || (posY != lastPosY))
		{
			// Check if the move is legal
			if (m_Moves[posX][posY])
			{
				// Move piece
				m_Board[posX][posY] = draggedPiece;
				// Set initial position to empty
				Piece p(-1, Type::Empty);
				m_Board[lastPosX][lastPosY] = p;
				m_Turn = !m_Turn;

				ResetLegalMoves();
			}
			else
			{
				// Return the piece to the initial possition if the move isn't legal
				m_Board[lastPosX][lastPosY] = draggedPiece;
			}
		}
		else
		{
			// Check for legal moves
			FindLegalMoves(posX, posY, draggedPiece);
		}
	}
	else
	{
		ResetLegalMoves();
	}
}	

void Board::UpdatePieceByCoordinates(int mousePosX, int mousePosY, 
	Piece draggedPiece, int lastClickX, int lastClickY)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	int lastPosX = lastClickX / m_TileWidth;
	int lastPosY = lastClickY / m_TileHeight;
	int pad = m_TileWidth * 0.1;
	int rectSize = m_TileWidth - (2 * pad);

	// Invalid position
	if (posX < 0 || posX > 7 || posY < 0 || posY > 7 || lastPosX < 0 
		|| lastPosX > 7 || lastPosY < 0 || lastPosY > 7) return;
	// Draw piece at mouse location

	if (draggedPiece.GetType() != Type::Empty)
	{
		if (m_Turn == draggedPiece.GetSide()) return;
		if ((posX != lastPosX) || (posY != lastPosY))
		{
			Piece p(-1, Type::Empty);
			m_Board[lastPosX][lastPosY] = p;

			SDL_Rect rect;
			rect.x = mousePosX - (rectSize / 2);
			rect.y = mousePosY - (rectSize / 2);
			rect.w = rectSize;
			rect.h = rectSize;

			SDL_RenderCopy(m_Renderer, draggedPiece.GetTexture(), NULL, &rect);
		}
		else
		{
			m_Board[lastPosX][lastPosY] = draggedPiece;
			FindLegalMoves(posX, posY, draggedPiece);
		}
	}
}

Piece Board::GetPiece(int mousePosX, int mousePosY)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	return m_Board[posX][posY];
}

void Board::FindLegalMoves(int x, int y, Piece piece)
{
	// Reset the matrix containning the possible moves
	ResetLegalMoves();

	if (piece.GetType() == Type::Empty())
		return;

	// Find all possible boxes the piece can move to based on the piece type
	if (piece.GetType() == Type::Pawn)
		PawnLegalMoves(x, y, piece);
	if (piece.GetType() == Type::Rook)
		RookLegalMoves(x, y, piece);
	if (piece.GetType() == Type::Knight)
		KnightLegalMoves(x, y, piece);
	if (piece.GetType() == Type::Bishop)
		BishopLegalMoves(x, y, piece);
	if (piece.GetType() == Type::Queen)
		QueenLegalMoves(x, y, piece);
	if (piece.GetType() == Type::King)
		KingLegalMoves(x, y, piece);
}

void Board::ResetLegalMoves()
{
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			m_Moves[i][j] = 0;
		}
	}
}

bool Board::LegalSquare(int x, int y, Piece piece)
{
	if (x >= 0 && x < m_Size && y >= 0 && y < m_Size)
	{
		if (m_Board[x][y].GetType() == Type::Empty())
			m_Moves[x][y] = 1;
		else
		{
			if (m_Board[x][y].GetSide() != piece.GetSide()
				&& m_Board[x][y].GetType() != Type::King)
			{
				m_Moves[x][y] = 2;
				return true;
			}
			else return true;
		}
	}
	return false;
}

void Board::PawnLegalMoves(int x, int y, Piece piece)
{
	int c = 0;
	if (piece.GetSide() == 0)
		c = y - 1;
	else 
		c = y + 1;

	if (m_Board[x][c].GetType() == Type::Empty)
		m_Moves[x][c] = 1;
	if (m_Board[x - 1][c].GetType() != Type::Empty
		&& m_Board[x - 1][c].GetSide() != piece.GetSide()
		&& m_Board[x - 1][c].GetType() != Type::King)
		m_Moves[x - 1][c] = 2;
	if (m_Board[x + 1][c].GetType() != Type::Empty
		&& m_Board[x + 1][c].GetSide() != piece.GetSide()
		&& m_Board[x + 1][c].GetType() != Type::King)
		m_Moves[x + 1][c] = 2;
}

void Board::RookLegalMoves(int x, int y, Piece piece)
{ 
	// right
	for (int i = x + 1; i < m_Size; i++)
	{
		if (LegalSquare(i, y, piece)) 
			break;
	}
	// left
	for (int i = x - 1; i >= 0; i--)
	{
		if (LegalSquare(i, y, piece)) 
			break;
	}
	// up
	for (int i = y - 1; i >= 0; i--)
	{
		if (LegalSquare(x, i, piece)) 
			break;
	}
	// down
	for (int i = y + 1; i < m_Size; i++)
	{
		if (LegalSquare(x, i, piece)) 
			break;
	}
}

void Board::KnightLegalMoves(int x, int y, Piece piece)
{ 
	int dirX[8] = { -2, -1,  1,  2,  2,  1, -1, -2 };
	int dirY[8] = { -1, -2, -2, -1,  1,  2,  2,  1 };

	int nX = 0;
	int nY = 0;

	for (int i = 0; i < 8; i++)
	{
		nX = x + dirX[i];
		nY = y + dirY[i];

		if (nX >= 0 && nX < m_Size && nY >= 0 && nY < m_Size)
		{
			if (m_Board[nX][nY].GetType() == Type::Empty())
				m_Moves[nX][nY] = 1;
			else
			{
				if (m_Board[nX][nY].GetSide() != piece.GetSide()
					&& m_Board[nX][nY].GetType() != Type::King)
				{
					m_Moves[nX][nY] = 2;
				}
			}
		}
	}
}

void Board::BishopLegalMoves(int x, int y, Piece piece)
{ 
	int i = x - 1;
	int j = y - 1;
	while (i >= 0 && j >= 0)
	{
		if (LegalSquare(i, j, piece))
			break;
		i--;
		j--;
	}
	i = x - 1;
	j = y + 1;
	while (i >= 0 && j < m_Size)
	{
		if (LegalSquare(i, j, piece))
			break;
		i--;
		j++;
	}
	i = x + 1;
	j = y - 1;
	while (i < m_Size && j >= 0)
	{
		if (LegalSquare(i, j, piece))
			break;
		i++;
		j--;
	}
	i = x + 1;
	j = y + 1;
	while (i < m_Size && j < m_Size)
	{
		if (LegalSquare(i, j, piece))
			break;
		i++;
		j++;
	}
}

void Board::QueenLegalMoves(int x, int y, Piece piece)
{
	RookLegalMoves(x, y, piece);
	BishopLegalMoves(x, y, piece);
	KingLegalMoves(x, y, piece);
}

void Board::KingLegalMoves(int x, int y, Piece piece)
{
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (x + i >= 0 && x + i < m_Size && y + j >= 0 && y + j < m_Size)
			{
				if (m_Board[x + i][y + j].GetType() == Type::Empty)
					m_Moves[x + i][y + j] = 1;
				else
				{
					if (m_Board[x + i][y + j].GetSide() != piece.GetSide()
						&& m_Board[x + i][y + j].GetType() != Type::King)
					{
						m_Moves[x + i][y + j] = 2;
					}
				}
			}
		}
	}
}


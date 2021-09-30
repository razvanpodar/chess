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
							1,	1,	1,	1,	1,	1,	1,	1,
							2,	3,	4,	5,	6,	4,	3,	2 };

Board::Board()
{ }

Board::Board(int screenWidth, int screenHeight)
{
	//m_BoardColor[0] = { 69, 190, 100 };
	//m_BoardColor[1] = { 100, 25, 150 };

	m_BoardColor[0] = { 220, 210, 150 };
	m_BoardColor[1] = { 120, 80, 10 };

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
	m_BoardColor[1] = { 120, 80, 10 };

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
			//int index = static_cast<int>(piece.GetType());
			//SDL_SetRenderDrawColor(m_Renderer, m_PixelColor[index].R, 
			//	m_PixelColor[index].B, m_PixelColor[index].G, 0);

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
				printf("%d\n", v);
				SDL_SetRenderDrawColor(m_Renderer, rectColor[v].R, rectColor[v].G, 
					rectColor[v].B, 0);

				//SDL_RenderSetScale(m_Renderer, 4.0, 4.0);

				SDL_Rect rect;
				rect.x = i * m_TileWidth;
				rect.y = j * m_TileHeight;
				rect.w = m_TileWidth;
				rect.h = m_TileHeight;

				SDL_RenderFillRect(m_Renderer, &rect);
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
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			m_Moves[i][j] = 0;
		}
	}

	if (piece.GetType() == Type::Empty())
	{
		return;
	}
	// Find all possible boxes the piece can move to
	if (piece.GetType() == Type::Pawn)
	{
		if (piece.GetSide() == 0)
		{
			if (m_Board[x][y - 1].GetType() == Type::Empty)
				m_Moves[x][y - 1] = 1;
			if (m_Board[x - 1][y - 1].GetType() != Type::Empty 
				&& m_Board[x - 1][y - 1].GetSide() == 1)
				m_Moves[x - 1][y - 1] = 2;
			if (m_Board[x + 1][y - 1].GetType() != Type::Empty
				&& m_Board[x - 1][y - 1].GetSide() == 1)
				m_Moves[x + 1][y - 1] = 2;

		}
		else
		{
			if (m_Board[x][y + 1].GetType() == Type::Empty)
				m_Moves[x][y + 1] = 1;
			if (m_Board[x - 1][y + 1].GetType() != Type::Empty
				&& m_Board[x - 1][y + 1].GetSide() == 0)
				m_Moves[x - 1][y + 1] = 2;
			if (m_Board[x + 1][y + 1].GetType() != Type::Empty
				&& m_Board[x - 1][y + 1].GetSide() == 0)
				m_Moves[x + 1][y + 1] = 2;
		}
	}

	if (piece.GetType() == Type::Rook)
	{

	}
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

#include "Board.h"

Board::Board(int screenWidth, int screenHeight)
	: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
	// Will be replaced with textures
	m_PixelColor[0] = { 0, 0, 0 };
	m_PixelColor[1] = { 255, 255, 255 };
	m_PixelColor[2] = { 255, 0, 0 };
	m_PixelColor[3] = { 0, 255, 0 };
	m_PixelColor[4] = { 0, 0, 255 };
	m_PixelColor[5] = { 255, 255, 0 };
	m_PixelColor[6] = { 255, 0, 255 };
	m_PixelColor[7] = { 0, 255, 255 };

	m_BoardColor[0] = { 69, 190, 100 };
	m_BoardColor[1] = { 100, 25, 150 };

	m_TileWidth = m_ScreenWidth / m_Size;
	m_TileHeight = m_ScreenHeight / m_Size;

	Init();
}

Board::~Board()
{

}

void Board::Init()
{
	// Initialize the location of each piece
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece p(0, Type::Empty);
			// First or last row
			if (j == 0 || j == m_Size - 1)
			{
				(j == 0) ? p.setSide(0) : p.setSide(1);
				// Could be done with a switch 
				// But the logic is better this way imo
				if (i == 0) p.setType(Type::Rook);
				if (i == 1) p.setType(Type::Knight);
				if (i == 2) p.setType(Type::Bishop);
				if (i == 3) p.setType(Type::Queen);
				if (i == 5) p.setType(Type::King);
				if (i == m_Size - 3) p.setType(Type::Bishop);
				if (i == m_Size - 2) p.setType(Type::Knight);
				if (i == m_Size - 1) p.setType(Type::Rook);
			}
			else
			{
				// Second or second to last row
				if (j == 1 || j == m_Size - 2)
				{
					(j == 1) ? p.setSide(0) : p.setSide(1);
					p.setType(Type::Pawn);
				}
			}
			m_Board[i][j] = p;
		}
	}
}

void Board::Update(SDL_Renderer *renderer, bool isDragged, int mousePosX, 
	int mousePosY, Piece draggedPiece)
{

	// Draw board
	DrawBoard(renderer);
	
	// if(isDragged)
	// replace piece with 0 in board
	// draw piece at cursor coordinates
	if (!isDragged) {
		UpdatePiece(renderer, mousePosX, mousePosY, draggedPiece);
	} 
	else
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		UpdatePieceByCoordinates(renderer, x, y, draggedPiece);
	}

	DrawPieces(renderer);

	// Make move

	// Draw pieces
}

void Board::DrawBoard(SDL_Renderer *renderer)
{
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			int c = (i + j) % 2;
			SDL_SetRenderDrawColor(renderer, m_BoardColor[c].R, m_BoardColor[c].G, 
				m_BoardColor[c].B, 0);
			SDL_Rect rect;
			rect.x = i * m_TileWidth;
			rect.y = j * m_TileHeight;
			rect.w = m_TileWidth;
			rect.h = m_TileHeight;

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Board::DrawPieces(SDL_Renderer* renderer)
{
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece piece = m_Board[i][j];
			int index = static_cast<int>(piece.getType());
			SDL_SetRenderDrawColor(renderer, m_PixelColor[index].R, 
				m_PixelColor[index].B, m_PixelColor[index].G, 0);
			int rectWidth = m_TileWidth - 20;
			int rectHeight = m_TileHeight - 20;

			SDL_Rect rect;
			rect.x = (i * m_TileWidth) + (rectWidth / 4);
			rect.y = (j * m_TileHeight) + (rectHeight / 4);
			rect.w = rectWidth;
			rect.h = rectHeight;

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Board::UpdatePiece(SDL_Renderer* renderer, int mousePosX, int mousePosY, 
	Piece draggedPiece)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	if (posX < 0 || posX > 7 || posY < 0 || posY > 7)
	{
		// Invalid position
		// printf("Invalid possition");
		return;
	}
	// Check if the location is a valid move

	// Move piece
	m_Board[posX][posY] = draggedPiece;
	// Set initial position to empty
}

void Board::UpdatePieceByCoordinates(SDL_Renderer* renderer, int mousePosX, 
	int mousePosY, Piece draggedPiece)
{
	// Draw piece at mouse location

}

Piece Board::GetPiece(int mousePosX, int mousePosY)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	return m_Board[posX][posY];
}

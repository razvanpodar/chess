#include "Board.h"
#include "TextureManager.h"

Board::Board()
{ }

Board::Board(int screenWidth, int screenHeight)
{
	m_BoardColor[0] = { 69, 190, 100 };
	m_BoardColor[1] = { 100, 25, 150 };

	m_TileWidth = m_ScreenWidth / m_Size;
	m_TileHeight = m_ScreenHeight / m_Size;
}

Board::~Board()
{
	SDL_DestroyRenderer(m_Renderer);
}

void Board::Init(SDL_Window *window)
{
	m_BoardColor[0] = { 69, 190, 100 };
	m_BoardColor[1] = { 100, 25, 150 };

	m_TileWidth = m_ScreenWidth / m_Size;
	m_TileHeight = m_ScreenHeight / m_Size;

	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TextureManager texMan;
	// Initialize the location of each piece
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece p(-1, Type::Empty);
			int index = 0;
			// First or last row
			if (j == 0 || j == m_Size - 1)
			{
				(j == 0) ? p.SetSide(1) : p.SetSide(0);
				// Could be done with a switch 
				// But the logic is better this way imo
				if (i == 0)
				{
					index = (p.GetSide() * 6) + 1;
					p.SetType(Type::Rook);
				}
				if (i == 1)
				{
					index = (p.GetSide() * 6) + 2;
					p.SetType(Type::Knight);
				}
				if (i == 2)
				{
					index = (p.GetSide() * 6) + 3;
					p.SetType(Type::Bishop);
				}
				if (i == 3)
				{
					index = (p.GetSide() * 6) + 4;
					p.SetType(Type::Queen);
				}
				if (i == 4)
				{
					index = (p.GetSide() * 6) + 5;
					p.SetType(Type::King);
				}
				if (i == m_Size - 3)
				{
					index = (p.GetSide() * 6) + 3;
					p.SetType(Type::Bishop);
				}
				if (i == m_Size - 2)
				{
					index = (p.GetSide() * 6) + 2;
					p.SetType(Type::Knight);
				}
				if (i == m_Size - 1)
				{
					index = (p.GetSide() * 6) + 1;
					p.SetType(Type::Rook);
				}
				p.SetTexture(texMan.LoadTexture(m_Renderer, (path + files[index]).c_str()));
			}
			else
			{
				// Second or second to last row
				if (j == 1 || j == m_Size - 2)
				{
					(j == 1) ? p.SetSide(1) : p.SetSide(0);
					index = p.GetSide() * 6;
					p.SetType(Type::Pawn);
				}
				p.SetTexture(texMan.LoadTexture(m_Renderer, (path + files[index]).c_str()));
			}
			//if (p.GetType() == Type::Empty) printf("Empty\n");
			m_Board[i][j] = p;
		}
	}
}

void Board::Update(bool isDragged, int mousePosX, 
	int mousePosY, Piece draggedPiece, int lastClickX, int lastClickY)
{

	// Draw board
	DrawBoard();
	//printf("%d, %d", lastClickX, lastClickY);
	// if(isDragged)
	// replace piece with 0 in board
	// draw piece at cursor coordinates
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

	// Draw pieces
	DrawPieces();
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
	float padPercent = 0.1;
	int pad = m_TileWidth * padPercent;
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			Piece piece = m_Board[i][j];

			if (piece.GetType() == Type::Empty) continue;
			//int index = static_cast<int>(piece.GetType());
			//SDL_SetRenderDrawColor(m_Renderer, m_PixelColor[index].R, 
			//	m_PixelColor[index].B, m_PixelColor[index].G, 0);
			int rectWidth = m_TileWidth - (2 * pad);
			int rectHeight = m_TileHeight - (2 * pad);

			SDL_Rect rect;
			rect.x = (i * m_TileWidth) + pad;
			rect.y = (j * m_TileHeight) + pad;
			rect.w = rectWidth;
			rect.h = rectHeight;

			SDL_RenderCopy(m_Renderer, piece.GetTexture(), NULL, &rect);

			//SDL_RenderFillRect(m_Renderer, &rect);
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
	if (posX < 0 || posX > 7 || posY < 0 || posY > 7 || lastPosX < 0 || lastPosX > 7 
		|| lastPosY < 0 || lastPosY > 7) return;
	// Check if the location is a valid move
	if (draggedPiece.GetType() != Type::Empty)
	{
		if ((posX != lastPosX) || (posY != lastPosY))
		{
			// Move piece
			//printf("%d, %d\n", posX, posY);
			//printf("%d, %d\n", lastPosX, lastPosY);
			m_Board[posX][posY] = draggedPiece;
			// Set initial position to empty
			Piece p(-1, Type::Empty);
			m_Board[lastPosX][lastPosY] = p;
		} 
		else
		{
			// Check for viable moves
			// Mark the possitions of all moves on the board
			printf("same pos\n");
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
	if (lastPosX < 0 || lastPosX > 7 || lastPosY < 0 || lastPosY > 7) return;
	// Draw piece at mouse location
	if (draggedPiece.GetType() != Type::Empty)
	{
		if ((posX == lastPosX) || (posY == lastPosY))
			m_Board[lastPosX][lastPosY] = draggedPiece;

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
	}
}

Piece Board::GetPiece(int mousePosX, int mousePosY)
{
	int posX = mousePosX / m_TileWidth;
	int posY = mousePosY / m_TileHeight;

	return m_Board[posX][posY];
}

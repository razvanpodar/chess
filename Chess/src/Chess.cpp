#include "Chess.h"

Chess::Chess(int WIDTH, int HEIGHT)
	: m_WindowWidth(WIDTH), m_WindowHeight(HEIGHT)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* m_Window;
	m_Window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, 0);
	
	// Check if window could be created
	if (m_Window == NULL)
	{
		printf("Failed to create window!");
	}

	m_Board.SetScreenWidthAndHeight(WIDTH, HEIGHT);
	m_Board.Init(m_Window);
}

Chess::~Chess()
{
	//free(m_Board);
	//free(m_Event);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Chess::Init()
{

}

void Chess::Update()
{
	if (SDL_PollEvent(&m_Event))
	{
		switch (m_Event.type)
		{
		case SDL_QUIT:
			SDL_Log("Program quit after %i ticks", m_Event.quit.timestamp);
			Quit();
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (m_Event.button.button)
			{
			case SDL_BUTTON_LEFT:
				SDL_GetMouseState(&x, &y);
				printf("Begin drag: %d, %d\n", x, y);
				draggedPiece = m_Board.GetPiece(x, y);
				isDragged = true;
				lastClickX = x;
				lastClickY = y;
				printf("%d, %d\n", lastClickX, lastClickY);
			default:
				break;
			}
		case SDL_MOUSEBUTTONUP:
			switch (m_Event.button.button)
			{
			case SDL_BUTTON_LEFT:
				if (m_Event.button.state == SDL_RELEASED)
				{
					SDL_GetMouseState(&x, &y);
					printf("End drag: %d, %d\n", x, y);
					isDragged = false;
				}
			default:
				break;
			}
		case SDL_KEYDOWN:
			switch (m_Event.key.keysym.sym)
			{
			case SDLK_r:
				draggedPiece = Piece(-1, Type::Empty);
				m_Board.Reset();
				m_Board.m_Turn = 1;
			default:
				break;
			}
		default:
			break;
		}
	}

	m_Board.Update(isDragged, x, y, draggedPiece, lastClickX, lastClickY);
	SDL_RenderClear(m_Board.m_Renderer);
}



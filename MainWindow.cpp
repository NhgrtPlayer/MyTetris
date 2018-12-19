#include <iostream>
#include "MainWindow.h"
#include "Tetrominos.h"

MainWindow::MainWindow()
{
	Initialized = false;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		std::cerr << "SDL Initialization failed (error: " << SDL_GetError() << ").\n";
		return;
	}
	pWindow = SDL_CreateWindow("MyTetris",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		MainWindow::Height,
		MainWindow::Width,
		SDL_WINDOW_SHOWN);
	if (!pWindow) {
		return;
	}
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!pRenderer) {
		return;
	}
	SDL_Surface *surface = SDL_CreateRGBSurface(0, MainWindow::CellSize * 12, MainWindow::CellSize * 25, 32, 0, 0, 0, 0);
	if (!surface) {
		return;
	}
	pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
	SDL_FreeSurface(surface);
	if (!pTexture) {
		return;
	}
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	Initialized = true;
}

MainWindow::~MainWindow()
{
	SDL_DestroyTexture(pTexture);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void GetColorByEnum(EnumColor Color, SDL_Renderer *pRenderer)
{
	switch (Color)
	{
	case EMPTY:
		SDL_SetRenderDrawColor(pRenderer, 0x0, 0x0, 0x0, 0xFF);
		break;
	case CYAN:
		SDL_SetRenderDrawColor(pRenderer, 0x0, 0xFF, 0xFF, 0xFF);
		break;
	case YELLOW:
		SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xD7, 0x0, 0xFF);
		break;
	case PURPLE:
		SDL_SetRenderDrawColor(pRenderer, 0x8A, 0x2B, 0xE2, 0xFF);
		break;
	case ORANGE:
		SDL_SetRenderDrawColor(pRenderer, 0xFF, 0x8C, 0x0, 0xFF);
		break;
	case BLUE:
		SDL_SetRenderDrawColor(pRenderer, 0x0, 0x0, 0x8B, 0xFF);
		break;
	case RED:
		SDL_SetRenderDrawColor(pRenderer, 0x8B, 0x0, 0x0, 0xFF);
		break;
	case GREEN:
		SDL_SetRenderDrawColor(pRenderer, 0x32, 0xCD, 0x32, 0xFF);
		break;
	default:
		SDL_SetRenderDrawColor(pRenderer, 0x0, 0x0, 0x0, 0xFF);
		break;
	}
}

void MainWindow::DrawGrid(const Field & field)
{
	int TotalWidth = (Field::Width + 2) * MainWindow::CellSize + (Field::Width - 1) * MainWindow::GridSize;
	SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0x30);
	SDL_Rect GridRect;

	// Vertical Lines
	GridRect.h = Field::Height * MainWindow::CellSize + (Field::Height - 1) * MainWindow::GridSize;
	GridRect.w = MainWindow::GridSize;
	GridRect.y = 0;

	for (int i = 0; i < Field::Width - 1; ++i) // X
	{
		GridRect.x = i * MainWindow::GridSize + (i + 1) * MainWindow::CellSize;
		GridRect.x += MainWindow::Width / 2 - TotalWidth / 2 + MainWindow::CellSize;
		SDL_RenderFillRect(pRenderer, &GridRect);
	}

	// Horizontal Lines
	GridRect.h = MainWindow::GridSize;
	GridRect.w = TotalWidth;
	GridRect.x = MainWindow::Width / 2 - TotalWidth / 2;

	for (int i = 0; i < Field::Height - 1; ++i) // Y
	{
		GridRect.y = (i + 1) * MainWindow::CellSize + i * MainWindow::GridSize;
		SDL_RenderFillRect(pRenderer, &GridRect);
	}
}

void MainWindow::UpdateAndShowWindow(const Field & field, const Tetrominos & ControlledPiece)
{
	int TotalWidth = (Field::Width + 2) * MainWindow::CellSize + (Field::Width - 1) * MainWindow::GridSize;
	Field FieldTmp = field;
	FieldTmp.SetTetrominos(ControlledPiece);
	SDL_SetRenderDrawColor(pRenderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(pRenderer);
	DrawGrid(field);

	SDL_Rect CellRect;

	CellRect.h = MainWindow::CellSize;
	CellRect.w = MainWindow::CellSize;

	for (int i = 0; i < Field::Height; ++i) // Y
	{
		for (int j = 0; j < Field::Width; ++j) // X
		{
			CellRect.x = j * MainWindow::CellSize + j * MainWindow::GridSize;
			CellRect.y = i * MainWindow::CellSize + i * MainWindow::GridSize;
			CellRect.x += MainWindow::Width / 2 - TotalWidth / 2 + MainWindow::CellSize;
			GetColorByEnum((EnumColor)FieldTmp.Grid[FieldTmp.GetIndexAt(j, i)], pRenderer);
			SDL_RenderFillRect(pRenderer, &CellRect);
		}
	}

	SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect rect;

	// Bottom border
	rect.h = MainWindow::CellSize;
	rect.w = TotalWidth;
	rect.x = MainWindow::Width / 2 - TotalWidth / 2;
	rect.y = MainWindow::CellSize * Field::Height + (MainWindow::GridSize * (Field::Height - 1));
	SDL_RenderFillRect(pRenderer, &rect);

	// Left border
	rect.h = MainWindow::CellSize * Field::Height + (MainWindow::GridSize * (Field::Height - 1));
	rect.w = MainWindow::CellSize;
	rect.y = 0;
	SDL_RenderFillRect(pRenderer, &rect);

	// Right border
	rect.x += TotalWidth - MainWindow::CellSize;
	SDL_RenderFillRect(pRenderer, &rect);
	SDL_RenderPresent(pRenderer);
}
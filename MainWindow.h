#pragma once

#include <SDL.h>
#include "Field.h"

class MainWindow
{
public:
	const static int Height = 640;
	const static int Width = 480;
	const static int CellSize = 16;
	const static int GridSize = 3;
	SDL_Window* pWindow = NULL;
	SDL_Renderer *pRenderer = NULL;
	SDL_Texture *pTexture = NULL;

	bool Initialized;

public:
	MainWindow();
	~MainWindow();

	void DrawGrid(const Field & field);
	void UpdateAndShowWindow(const Field & field, const Tetrominos & ControlledPiece);
};


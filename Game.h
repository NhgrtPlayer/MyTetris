#pragma once

#include "Tetrominos.h"
#include "Field.h"
#include "MainWindow.h"

class Game
{
	bool ExitGame;
	bool GameOver;

	float Speed;
	const static float FrameRate; // Défini dans Game.cpp
	std::vector<Tetrominos> PossiblePieces;

	Tetrominos ControlledPiece;
	Tetrominos NextPiece;
	Field field;

	int Level = 0;
	int LinesCompleted = 0;
	int Score = 0;

	//SDL
	MainWindow mainWindow;
	SDL_Event Event;
public:
	Game();
	~Game();

	void SetSpeedByLevel();
	void CreatePossiblePieces();
	Tetrominos GetRandomPiece() const;
	bool SpawnNewPiece();
	int GetScoreGain(int LinesRemoved) const;

	void HandleEvents();
	void UpdateControlledPiece();
	void HandleLinesToRemove();
	void UpdateGame();

	int GameLoop();
};


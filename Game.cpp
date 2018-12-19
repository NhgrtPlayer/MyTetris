#include <ctime>
#include "Game.h"


const float Game::FrameRate = 60.f;

Game::Game()
{
	srand(time(NULL));
	ExitGame = false;
	GameOver = false;
	Level = 0;
	SetSpeedByLevel();
	CreatePossiblePieces();
	NextPiece = GetRandomPiece();
	GameOver = !SpawnNewPiece();
}


Game::~Game()
{
}

void Game::SetSpeedByLevel()
{
	std::array<float, 29> Speeds = {
		48.f, //0
		43.f, //1
		38.f, //2
		33.f, //3
		28.f, //4
		23.f, //5
		18.f, //6
		13.f, //7
		8.f, //8
		6.f, //9
		5.f, //10
		5.f, //11
		5.f, //12
		4.f, //13
		4.f, //14
		4.f, //15
		3.f, //16
		3.f, //17
		3.f, //18
	};
	for (int i = 19; i <= 28; ++i)
	{
		Speeds[i] = 2.f;
	}
	if (Level >= 29)
	{
		Speed = 1.f;
		return;
	}
	Speed = Speeds[Level];
}

void Game::CreatePossiblePieces()
{
	Tetrominos I(4, 1, 1, EnumColor::CYAN, { "1111" });
	Tetrominos J(3, 2, 4, EnumColor::BLUE, { "100111", "111010", "111001", "010111" });
	Tetrominos L(3, 2, 4, EnumColor::ORANGE, { "001111", "101011", "111100", "110101" });
	Tetrominos O(2, 2, 0, EnumColor::YELLOW, { "1111" });
	Tetrominos S(3, 2, 4, EnumColor::GREEN, { "011110", "101101" });
	Tetrominos T(3, 2, 4, EnumColor::PURPLE, { "010111", "101110", "111010", "011101" });
	Tetrominos Z(3, 2, 4, EnumColor::RED, { "110011", "011110" });

	PossiblePieces.push_back(I);
	PossiblePieces.push_back(J);
	PossiblePieces.push_back(L);
	PossiblePieces.push_back(O);
	PossiblePieces.push_back(S);
	PossiblePieces.push_back(T);
	PossiblePieces.push_back(Z);
}

Tetrominos Game::GetRandomPiece() const
{
	int RandomIndex = std::rand() % PossiblePieces.size();
	return (PossiblePieces[RandomIndex]);
}

bool Game::SpawnNewPiece()
{
	if (!field.IsEmptyAt(Field::Width / 2, 0))
	{
		return (false);
	}
	ControlledPiece = NextPiece;
	NextPiece = GetRandomPiece();
	ControlledPiece.X = Field::Width / 2;
	ControlledPiece.Y = 0;
	return (ControlledPiece.CanMoveHere(field, ControlledPiece.X, ControlledPiece.Y));
}

int Game::GetScoreGain(int LinesRemoved) const
{
	switch (LinesRemoved)
	{
	case 1:
		return (40 * (Level + 1));
	case 2:
		return (100 * (Level + 1));
	case 3:
		return (300 * (Level + 1));
	case 4:
		return (1200 * (Level + 1));
	}
	return (0);
}

void Game::HandleEvents()
{
	ExitGame = false;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_WINDOWEVENT: // Événement de la fenêtre
			if (Event.window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
			{
				// Action à faire lorsque l'utilisateur clique sur la croix
				ExitGame = true;
				return;
			}
			break;
		case SDL_KEYUP: // Événement de relâchement d'une touche clavier
			if (Event.key.keysym.sym == SDLK_ESCAPE) // C'est la touche Échap
			{
				ExitGame = true;
				return;
				// Action à faire lorsque l'utilisateur relâche la touche Échap
			}
			break;
		case SDL_KEYDOWN:
			if (Event.key.keysym.sym == SDLK_LEFT && ControlledPiece.CanMoveHere(field, ControlledPiece.X - 1, ControlledPiece.Y))
			{ // Aller à gauche
				ControlledPiece.X--;
			}
			if (Event.key.keysym.sym == SDLK_RIGHT && ControlledPiece.CanMoveHere(field, ControlledPiece.X + 1, ControlledPiece.Y))
			{ // Aller à droite
				ControlledPiece.X++;
			}
			if (Event.key.keysym.sym == SDLK_UP && ControlledPiece.CanRotateClockwise(field))
			{ // Rotation horaire
				ControlledPiece.RotateClockwise();
			}
			if (Event.key.keysym.sym == SDLK_SPACE)
			{ // HardDrop
				ControlledPiece.HardDrop(field);
			}
			break;
		}
	}
}

void Game::UpdateControlledPiece()
{
	if (ControlledPiece.CanFallDown(field))
	{
		//ShowField(field, ControlledPiece);
		ControlledPiece.Y++;
	}
	else
	{
		field.SetTetrominos(ControlledPiece);
		HandleLinesToRemove();
		SetSpeedByLevel();
		GameOver = !SpawnNewPiece();
	}
}

void Game::HandleLinesToRemove()
{
	int LinesRemoved = field.RemoveLines();
	Score += GetScoreGain(LinesRemoved);
	LinesCompleted += LinesRemoved;
	if (LinesCompleted >= (Level + 1) * 10) {
		Level++;
	}
}

void Game::UpdateGame()
{
	if (!GameOver)
	{
		HandleEvents();
		if (ExitGame) {
			return;
		}
		UpdateControlledPiece();
		mainWindow.UpdateAndShowWindow(field, ControlledPiece);
		SDL_Delay(Speed / FrameRate * 1000);
	}
}

int Game::GameLoop()
{
	while (!GameOver)
	{
		UpdateGame();
		if (ExitGame) {
			return (1);
		}
	}
	return (0);
}

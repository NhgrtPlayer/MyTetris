#include <iostream>
#include <ctime>
#include <vector>

#include "Field.h"
#include "Tetrominos.h"
#include "MainWindow.h"
#include "Game.h"

float WAITING_TIME = 48.f;
const float FRAME_RATE = 60.f;

void CreatePossiblePieces(std::vector<Tetrominos> & PossiblePieces)
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

void ShowField(const Field & field, const Tetrominos & ControlledPiece)
{
	Field field_tmp = field;

	std::cout << field_tmp.GetIndexAt(ControlledPiece.X, ControlledPiece.Y) << '\n';

	for (int i = 0; i < ControlledPiece.Height; ++i) // Y
	{
		for (int j = 0; j < ControlledPiece.Width; ++j) // X
		{
			int ControlledPieceIndexOnField = field_tmp.GetIndexAt(ControlledPiece.X + j, ControlledPiece.Y + i);
			/*std::cout << "I : " << i << " (" << ControlledPiece.X + j << ", " << ControlledPiece.Y + i << ")\n";
			std::cout << "index : " << ControlledPieceIndexOnField << '\n';*/
			field_tmp.Grid[ControlledPieceIndexOnField] = ControlledPiece.Forms[ControlledPiece.RotationIndex][ControlledPiece.GetIndexAt(j, i)];
		}
	}
	field_tmp.Print();
}

Tetrominos GetRandomPiece(const std::vector<Tetrominos> & PossiblePieces)
{
	int RandomIndex = std::rand() % PossiblePieces.size();
	return (PossiblePieces[RandomIndex]);
}

bool SpawnNewPiece(const Field & field, const std::vector<Tetrominos> & PossiblePieces, Tetrominos & TetrominosToUse)
{
	if (!field.IsEmptyAt(Field::Width / 2, 0))
	{
		return (false);
	}
	TetrominosToUse = GetRandomPiece(PossiblePieces);
	TetrominosToUse.X = Field::Width / 2;
	TetrominosToUse.Y = 0;
	return (TetrominosToUse.CanMoveHere(field, TetrominosToUse.X, TetrominosToUse.Y));
}

int ScoreGained(int Level, int LinesRemoved)
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

float GetNewSpeed(int Level)
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
	if (Level < 29) {
		return (Speeds[Level]);
	}
	return (1.f);
}

int main(int ac, char **av)
{
	Game game;

	return (game.GameLoop());
	/*srand(time(NULL));
	std::vector<Tetrominos> PossiblePieces;
	Field field;

	CreatePossiblePieces(PossiblePieces);
	Tetrominos ControlledPiece;
	int Level = 0;
	int LinesCompleted = 0;
	int Score = 0;
	MainWindow mainWindow;

	if (!mainWindow.Initialized) {
		return (-1);
	}

	SDL_Event ev;
	bool GameOver = !SpawnNewPiece(field, PossiblePieces, ControlledPiece);

	mainWindow.UpdateAndShowWindow(field, ControlledPiece);

	while (!GameOver)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_WINDOWEVENT: // Événement de la fenêtre
				if (ev.window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
				{
					// Action à faire lorsque l'utilisateur clique sur la croix
					return (0);
				}
				break;
			case SDL_KEYUP: // Événement de relâchement d'une touche clavier
				if (ev.key.keysym.sym == SDLK_ESCAPE) // C'est la touche Échap
				{
					return (0);
					// Action à faire lorsque l'utilisateur relâche la touche Échap
				}
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_LEFT && ControlledPiece.CanMoveHere(field, ControlledPiece.X - 1, ControlledPiece.Y))
				{
					ControlledPiece.X--;
				}
				if (ev.key.keysym.sym == SDLK_RIGHT && ControlledPiece.CanMoveHere(field, ControlledPiece.X + 1, ControlledPiece.Y))
				{
					ControlledPiece.X++;
				}
				if (ev.key.keysym.sym == SDLK_UP && ControlledPiece.CanRotateClockwise(field))
				{
					ControlledPiece.RotateClockwise();
				}
				if (ev.key.keysym.sym == SDLK_SPACE)
				{
					ControlledPiece.HardDrop(field);
				}
				break;
			}
		}
		mainWindow.UpdateAndShowWindow(field, ControlledPiece);
		if (ControlledPiece.CanFallDown(field))
		{
			//ShowField(field, ControlledPiece);
			ControlledPiece.Y++;
		}
		else
		{
			field.SetTetrominos(ControlledPiece);
			int LinesRemoved = field.RemoveLines();
			Score += ScoreGained(Level, LinesRemoved);
			LinesCompleted += LinesRemoved;
			if (LinesCompleted >= (Level + 1) * 10) {
				Level++;
			}
			WAITING_TIME = GetNewSpeed(Level);
			GameOver = !SpawnNewPiece(field, PossiblePieces, ControlledPiece);
		}
		mainWindow.UpdateAndShowWindow(field, ControlledPiece);
		SDL_Delay(WAITING_TIME / FRAME_RATE * 1000);
	}
	mainWindow.UpdateAndShowWindow(field, ControlledPiece);
	//ShowField(field, ControlledPiece);*/

	return (0);
}
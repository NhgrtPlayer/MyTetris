#include <iostream>
#include "Field.h"
#include "Tetrominos.h"

Field::Field()
{
	for (int i = 0; i < Field::Width * Field::Height; ++i)
	{
		Grid[i] = 0;
	}
}

Field::~Field()
{}

Field::Field(const Field & rhs)
{
	Grid = rhs.Grid;
}

Field & Field::operator = (const Field & rhs)
{
	Grid = rhs.Grid;
	return (*this);
}

int Field::GetIndexAt(int X, int Y) const
{
	return (Field::Width * Y + X);
}

void Field::Print() const
{
	for (int i = 0; i < Field::Height; ++i) // Y
	{
		std::cout << '|';
		for (int j = 0; j < Field::Width; ++j) // X
		{
			std::cout << Grid[GetIndexAt(j, i)];
		}
		std::cout << '|';
		std::cout << '\n';
	}
	std::cout << "------------";
	std::cout << '\n';
}

bool Field::IsEmptyAt(int X, int Y) const
{
	return (Grid[GetIndexAt(X, Y)] == 0);
}

void Field::SetTetrominos(const Tetrominos & PieceToSet)
{
	for (int i = 0; i < PieceToSet.Height; ++i) // Y
	{
		for (int j = 0; j < PieceToSet.Width; ++j) // X
		{
			if ((PieceToSet.Forms[PieceToSet.RotationIndex])[PieceToSet.GetIndexAt(j, i)] != 0) {
				int ControlledPieceIndexOnField = GetIndexAt(PieceToSet.X + j, PieceToSet.Y + i);
				Grid[ControlledPieceIndexOnField] = PieceToSet.Forms[PieceToSet.RotationIndex][PieceToSet.GetIndexAt(j, i)];
			}
		}
	}
}

void Field::SetTetrominosAt(int X, int Y, const Tetrominos & PieceToSet)
{
	for (int i = 0; i < PieceToSet.Height; ++i) // Y
	{
		for (int j = 0; j < PieceToSet.Width; ++j) // X
		{
			if (PieceToSet.Forms[PieceToSet.RotationIndex][PieceToSet.GetIndexAt(j, i)] != 0) {
				int ControlledPieceIndexOnField = GetIndexAt(X + j, Y + i);
				Grid[ControlledPieceIndexOnField] = PieceToSet.Forms[PieceToSet.RotationIndex][PieceToSet.GetIndexAt(j, i)];
			}
		}
	}
}

void Field::RemoveLine(int Y)
{
	for (int i = Y; i >= 0; --i) // Y
	{
		for (int j = 0; j < Field::Width; ++j) // X
		{
			if (i == 0) {
				Grid[GetIndexAt(j, i)] = 0;
			}
			else {
				Grid[GetIndexAt(j, i)] = Grid[GetIndexAt(j, i - 1)];
			}
		}
	}
}

int Field::RemoveLines()
{
	int ToReturn = 0;
	int i = Field::Height - 1;

	while (i >= 0) // Y
	{
		int NotEmpty = 0;
		for (int j = 0; j < Field::Width; ++j) // X
		{
			NotEmpty += !IsEmptyAt(j, i);
		}
		if (NotEmpty == Field::Width) {
			ToReturn++;
			RemoveLine(i);
		}
		else {
			i--;
		}
	}
	return (ToReturn);
}

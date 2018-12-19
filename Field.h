#pragma once

#include <array>

class Tetrominos;

class Field
{
public:
	const static int Width = 10;
	const static int Height = 24;
	std::array<int, Field::Width * Field::Height> Grid;
public:
	Field();
	~Field();
	Field(const Field & rhs);
	Field & operator = (const Field & rhs);

	int GetIndexAt(int X, int Y) const;
	void Print() const;
	bool IsEmptyAt(int X, int Y) const;
	void SetTetrominos(const Tetrominos & PieceToSet);
	void SetTetrominosAt(int X, int Y, const Tetrominos & PieceToSet);

	void RemoveLine(int Y);
	int RemoveLines();
};

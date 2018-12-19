#pragma once

#include <vector>
#include <string>
#include "Field.h"

enum EnumColor
{
	EMPTY,
	CYAN,
	YELLOW,
	PURPLE,
	ORANGE,
	BLUE,
	RED,
	GREEN,
};

class Tetrominos
{
public:
	int Width;
	int Height;
	int OriginIndex;
	std::vector<std::vector<int>> Forms;

	int X;
	int Y;
	int RotationIndex;
public:
	Tetrominos() {}
	Tetrominos(int NewWidth, int NewHeight, int NewOriginIndex, int Color, std::vector<std::string> NewForms);
	~Tetrominos();

	Tetrominos(const Tetrominos & rhs);
	Tetrominos & operator = (const Tetrominos & rhs);

	int GetIndexAt(int X, int Y) const;

	bool CanFallDown(const Field & field) const;
	bool CanMoveHere(const Field & field, int NewX, int NewY) const;
	bool CanRotateClockwise(const Field & field) const;

	void RotateClockwise();

	void HardDrop(const Field & field);
};
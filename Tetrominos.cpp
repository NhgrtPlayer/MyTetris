#include <iostream>
#include "Tetrominos.h"

Tetrominos::Tetrominos(int NewWidth, int NewHeight, int NewOriginIndex, int Color, std::vector<std::string> NewForms) :
	Width(NewWidth),
	Height(NewHeight),
	OriginIndex(NewOriginIndex),
	X(0),
	Y(0),
	RotationIndex(0)
{
	for (auto const &Form : NewForms)
	{
		std::vector<int> ToAdd;

		for (auto const e : Form)
		{
			if (e == '0')
				ToAdd.push_back(0);
			else
				ToAdd.push_back(Color);
		}
		Forms.push_back(ToAdd);
	}
}

Tetrominos::~Tetrominos()
{}

Tetrominos::Tetrominos(const Tetrominos & rhs)
{
	Width = rhs.Width;
	Height = rhs.Height;
	OriginIndex = rhs.OriginIndex;
	Forms = rhs.Forms;
	X = rhs.X;
	Y = rhs.Y;
	RotationIndex = rhs.RotationIndex;
}

Tetrominos & Tetrominos::operator = (const Tetrominos & rhs)
{
	Width = rhs.Width;
	Height = rhs.Height;
	OriginIndex = rhs.OriginIndex;
	Forms = rhs.Forms;
	X = rhs.X;
	Y = rhs.Y;
	RotationIndex = rhs.RotationIndex;
	return (*this);
}
int Tetrominos::GetIndexAt(int X, int Y) const
{
	return (this->Width * Y + X);
}

bool Tetrominos::CanFallDown(const Field & field) const
{
	return (CanMoveHere(field, X, Y + 1));
}

bool Tetrominos::CanMoveHere(const Field & field, int NewX, int NewY) const
{
	if (NewX < 0 || NewY < 0 || NewX + Width > Field::Width || NewY + Height > Field::Height) {
		return (false);
	}
	for (int i = 0; i < Height; ++i) // Y
	{
		for (int j = 0; j < Width; ++j) // X
		{
			if (Forms[RotationIndex][GetIndexAt(j, i)] != 0 && !field.IsEmptyAt(NewX + j, NewY + i))
			{
				return (false);
			}
		}
	}
	return (true);
}

bool Tetrominos::CanRotateClockwise(const Field & field) const
{
	Tetrominos Tmp = *this;

	Tmp.Height = Width;
	Tmp.Width = Height;
	Tmp.RotationIndex = (Tmp.RotationIndex + 1) % Tmp.Forms.size();
	if (Tmp.OriginIndex % 2 == 0) {
		Tmp.X++;
	}
	return (Tmp.CanMoveHere(field, Tmp.X, Tmp.Y));
}

void Tetrominos::RotateClockwise()
{
	int Tmp = Width;
	Width = Height;
	Height = Tmp;
	RotationIndex = (RotationIndex + 1) % Forms.size();
	if (OriginIndex % 2 == 0) {
		X++;
	}
}

void Tetrominos::HardDrop(const Field & field)
{
	while (CanFallDown(field))
	{
		Y++;
	}
}

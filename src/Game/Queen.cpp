#include "Queen.h"
#include "GLFW/Window.h"

Queen::Queen(const Color color, Point coord, float cellWidth, GL::Program *shader)
{
	pieceColor = color;
	currentPos = coord;
	std::string texturePath;
	if (color == Color::White)
	{
		texturePath = "textures/WhiteMarble";
	}
	else
	{
		texturePath = "textures/BlackMarble";
	}
}

Queen::Queen(const Color color, Point coord, float cellWidth, GL::Model *model)
{
	pieceColor = color;
	currentPos = coord;
	std::string texturePath;
	if (color == Color::White)
	{
		texturePath = "textures/WhiteMarble";
	}
	else
	{
		texturePath = "textures/BlackMarble";
	}

	model_ = model;
}

bool Queen::rookMovement(Cell* map[8][8], Point coord)
{
	if ((currentPos.X - coord.X == 0) && (currentPos.Y - coord.Y != 0))
	{
		int delta = (coord.Y - currentPos.Y) / abs(coord.Y - currentPos.Y);
		int pos = currentPos.Y + delta;
		while (pos != coord.Y + delta)
		{
			if (map[pos][currentPos.X]->mPiece != nullptr)
			{
				return false;
			}

			pos += delta;
		}

		return true;
	}

	if ((currentPos.X - coord.X != 0) && (currentPos.Y - coord.Y == 0))
	{
		int delta = (coord.X - currentPos.X) / abs(coord.X - currentPos.X);
		int pos = currentPos.X + delta;
		while (pos != coord.X + delta)
		{
			if (map[currentPos.Y][pos]->mPiece != nullptr)
			{
				return false;
			}

			pos += delta;
		}

		return true;
	}

	return false;
}

bool Queen::officerMovement(Cell* map[8][8], Point coord)
{
	if (abs(coord.X - currentPos.X) == abs(coord.Y - currentPos.Y))
	{
		int deltaX = (coord.X - currentPos.X) / abs(coord.X - currentPos.X);
		int deltaY = (coord.Y - currentPos.Y) / abs(coord.Y - currentPos.Y);
		int posX = currentPos.X + deltaX;
		int posY = currentPos.Y + deltaY;
		while (posX != coord.X + deltaX)
		{
			if (map[posY][posX]->mPiece != nullptr)
			{
				return false;
			}

			posX += deltaX;
			posY += deltaY;
		}

		return true;
	}

	return false;
}

bool Queen::MakeMove(Cell* map[8][8], Point coord)
{
	return rookMovement(map, coord) || officerMovement(map, coord);
}

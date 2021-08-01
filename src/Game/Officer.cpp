#include "Officer.h"
#include "GLFW/Window.h"

Officer::Officer(const Color color, Point coord, float cellWidth, GL::Program *shader)
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

	model_ = new GL::Model("models/chess_models/bishop.obj", texturePath, shader);
}

Officer::Officer(const Color color, Point coord, float cellWidth, GL::Model *model)
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

bool Officer::MakeMove(Cell* map[8][8], Point coord)
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
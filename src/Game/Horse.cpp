#include "Horse.h"
#include "GLFW/Window.h"

Horse::Horse(const Color color, Point coord, float cellWidth, GL::Program *shader)
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

Horse::Horse(const Color color, Point coord, float cellWidth, GL::Model *model)
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

bool Horse::MakeMove(Cell* map[8][8], Point coord)
{
	if ((abs(coord.X - currentPos.X) == 1 && abs(coord.Y - currentPos.Y) == 2) ||
		(abs(coord.X - currentPos.X) == 2 && abs(coord.Y - currentPos.Y) == 1))
	{
		return true;
	}
	
	return false;
}
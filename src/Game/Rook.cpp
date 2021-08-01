#include "Rook.h"
#include "GLFW/Window.h"

Rook::Rook(const Color color, Point coord, float cellWidth, GL::Program *shader)
{
	pieceColor = color;
	currentPos = coord;
	WasMoved = currentPos.X;
	std::string texturePath;
	if (color == Color::White)
	{
		texturePath = "textures/WhiteMarble";
	}
	else
	{
		texturePath = "textures/BlackMarble";
	}

	model_ = new GL::Model("models/chess_models/rook.obj", texturePath, shader);
}

Rook::Rook(const Color color, Point coord, float cellWidth, GL::Model *model)
{
	pieceColor = color;
	currentPos = coord;
	WasMoved = currentPos.X;
	model_ = model;
}

bool Rook::MakeMove(Cell* map[8][8], Point coord)
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

		WasMoved = true;
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

		WasMoved = true;
		return true;
	}
	
	return false;
}

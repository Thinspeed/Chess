#include "King.h"
#include "Rook.h"
#include "GLFW/Window.h"

King::King(const Color color, Point pos, float cellWidth, GL::Program *shader)
{
	pieceColor = color;
	currentPos = pos;
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

King::King(const Color color, Point pos, float cellWidth, GL::Model *model)
{
	pieceColor = color;
	currentPos = pos;
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

bool King::MakeMove(Cell* map[8][8], Point pos)
{
	if (((pos.X - currentPos.X == 0) || abs(pos.X - currentPos.X) == 1) &&
		((pos.Y - currentPos.Y == 0) || abs(pos.Y - currentPos.Y) == 1))
	{
		if (map[pos.Y][pos.X]->mPiece == nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (map[i][j]->mPiece != nullptr && map[i][j]->mPiece->pieceColor != pieceColor && map[i][j]->mPiece->TryToKillPiece(map, pos))
					{
						return false;
					}
				}
			}

			WasMoved = true;
			return true;
		}
	}
	
	return false;
}
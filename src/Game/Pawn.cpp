#include "Pawn.h"
#include "GLFW/Window.h"

Pawn::Pawn(const Color color, Point pos, float cellWidth, GL::Program *shader)
{
	pieceColor = color;
	currentPos = pos;
	isFirstStep = true;
	std::string texturePath;
	if (color == Color::White)
	{
		texturePath = "textures/WhiteMarble";
	}
	else
	{
		texturePath = "textures/BlackMarble";
	}

	model_ = new GL::Model("models/chess_models/pawn.obj", texturePath, shader);
}

Pawn::Pawn(const Color color, Point pos, float cellWidth, GL::Model *model)
{
	pieceColor = color;
	currentPos = pos;
	isFirstStep = true;
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

bool Pawn::MakeMove(Cell* map[8][8], Point pos)
{
	if (((pos.Y - currentPos.Y) * (int)pieceColor <= 1 + isFirstStep) &&
		(pos.X - currentPos.X == 0) && (map[pos.Y][pos.X]->mPiece == nullptr))
	{
		twoCellStep = isFirstStep && (abs(pos.Y - currentPos.Y) == 2);
		isFirstStep = false;
		return true;
	}

	return TryToKillPiece(map, pos);
}

bool Pawn::TryToKillPiece(Cell* map[8][8], Point pos)
{
	if (((pos.Y - currentPos.Y) * (int)pieceColor == 1) && (abs(pos.X - currentPos.X) == 1))
	{
		Piece* pieceToKill = map[pos.Y - (int)pieceColor][pos.X]->mPiece;
		if ((map[pos.Y][pos.X]->mPiece != nullptr))
		{
			return true;
		}
		if ((dynamic_cast<Pawn*>(pieceToKill) && dynamic_cast<Pawn*>(pieceToKill)->twoCellStep))
		{
			map[pos.Y][pos.X]->mPiece = pieceToKill;
			map[pos.Y - (int)pieceColor][pos.X]->mPiece = nullptr;
			return true;
		}
	}

	return false;
}
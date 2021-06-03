#include "Pawn.h"
#include "GLFW/Window.h"

Pawn::Pawn(const Color color, Point pos, float cellWidth)
{
	pieceColor = color;
	currentPos = pos;
	vao_.addVertexBufferObject({
		{ 0, 0, -1 },
		{ 0, cellWidth, -1 },
		{ cellWidth, cellWidth, -1 },
		{ cellWidth, 0, -1 },
	});

	vao_.addVertexBufferObject({
		{ 0, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 0, (0.25f * (-(int)color + 1)) },
		{ 1.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 1.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
	});
	
	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
	isFirstStep = true;
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
#include "King.h"
#include "GLFW/Window.h"

King::King(const Color color, Point coord, float cellWidth)
{
	pieceColor = color;
	currentPos = coord;
	vao_.addVertexBufferObject({
		{ 0, 0, -1 },
		{ 0, cellWidth, -1 },
		{ cellWidth, cellWidth, -1 },
		{ cellWidth, 0, -1 },
	});

	vao_.addVertexBufferObject({
		{ 5.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 5.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 1.0f, (0.25f * (-(int)color + 1)) },
		{ 1.0f, (0.5f + (0.25f * (-(int)color + 1))) },
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
}

bool King::MakeMove(Cell* map[8][8], Point coord)
{
	if (((coord.X - currentPos.X == 0) || abs(coord.X - currentPos.X) == 1) &&
		((coord.Y - currentPos.Y == 0) || abs(coord.Y - currentPos.Y) == 1))
	{
		if (map[coord.Y][coord.X]->mPiece == nullptr)
		{
			return true;
		}
	}
	
	return false;
}
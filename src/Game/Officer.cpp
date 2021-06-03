#include "Officer.h"
#include "GLFW/Window.h"

Officer::Officer(const Color color, Point coord, float cellWidth)
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
		{ 3.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 3.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 4.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 4.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
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
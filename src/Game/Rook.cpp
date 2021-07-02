#include "Rook.h"
#include "GLFW/Window.h"

Rook::Rook(const Color color, Point coord, float cellWidth)
{
	pieceColor = color;
	currentPos = coord;
	WasMoved = currentPos.X;
	vao_.addVertexBufferObject({
		{ 0, 0, -1 },
		{ 0, cellWidth, -1 },
		{ cellWidth, cellWidth, -1 },
		{ cellWidth, 0, -1 },
	});

	vao_.addVertexBufferObject({
		{ 1.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 1.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 2.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 2.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
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

#include "King.h"
#include "Rook.h"
#include "GLFW/Window.h"

King::King(const Color color, Point pos, float cellWidth)
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
		{ 5.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 5.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 1.0f, (0.25f * (-(int)color + 1)) },
		{ 1.0f, (0.5f + (0.25f * (-(int)color + 1))) },
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
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
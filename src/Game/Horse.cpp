#include "Horse.h"
#include "GLFW/Window.h"

Horse::Horse(const Color color, Point coord, float cellWidth)
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
		{ 2.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
		{ 2.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 3.0f / 6, (0.25f * (-(int)color + 1)) },
		{ 3.0f / 6, (0.5f + (0.25f * (-(int)color + 1))) },
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
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
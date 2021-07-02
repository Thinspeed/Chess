#pragma once
#include "GL/VAO.h"
#include "glm/glm.hpp"

enum class Color
{
	White = 1,
	Black = -1,
};

struct Point
{
	int X;
	int Y;

	Point()
	{
		X = 0;
		Y = 0;
	}
	
	Point(int x, int y)
	{
		X = x;
		Y = y;
	}

	bool operator == (Point value)
	{
		return (X == value.X) && (Y == value.Y);
	}

	bool operator != (Point value)
	{
		return !(Point(X, Y) == value);
	}
};

class Cell;

class Piece
{
protected:
	GL::VAO vao_;
public:
	bool isSelected = false;
	Color pieceColor;
	Point currentPos;
	virtual void Draw(GLuint modelMatrixID, float cellWidth);
	virtual bool MakeMove(Cell* map[8][8], Point pos);
	virtual bool TryToKillPiece(Cell* map[8][8], Point pos);
};
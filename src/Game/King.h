#pragma once
#include "Piece.h"
class King :
    public Piece
{
private:
public:
	King(const Color color, Point coord, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};


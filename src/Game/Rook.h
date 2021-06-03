#pragma once
#include "Piece.h"

class Rook :
    public Piece
{
private:
public:
	Rook(const Color color, Point coord, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
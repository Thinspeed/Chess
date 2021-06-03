#pragma once
#include "Piece.h"
class Horse :
    public Piece
{
private:
public:
	Horse(const Color color, Point coord, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
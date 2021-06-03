#pragma once
#include "Piece.h"

class Officer :
    public Piece
{
private:
public:
	Officer(const Color color, Point coord, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
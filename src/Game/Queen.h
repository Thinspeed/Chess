#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
private:
	bool rookMovement(Cell* map[8][8], Point coord);
	bool officerMovement(Cell* map[8][8], Point coord);
public:
	Queen(const Color color, Point coord, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
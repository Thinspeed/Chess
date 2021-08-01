#pragma once
#include "Piece.h"
#include "GL/Program.h"

class Queen :
    public Piece
{
private:
	bool rookMovement(Cell* map[8][8], Point coord);
	bool officerMovement(Cell* map[8][8], Point coord);
public:
	Queen(const Color color, Point coord, float cellWidth, GL::Program *shader);
	Queen(const Color color, Point coord, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
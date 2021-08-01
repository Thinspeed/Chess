#pragma once
#include "Piece.h"
#include "GL/Program.h"

class Horse :
    public Piece
{
private:
public:
	Horse(const Color color, Point coord, float cellWidth, GL::Program *shader);
	Horse(const Color color, Point coord, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
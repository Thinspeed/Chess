#pragma once
#include "Piece.h"
#include "GL/Program.h"

class Officer :
    public Piece
{
private:
public:
	Officer(const Color color, Point coord, float cellWidth, GL::Program *shader);
	Officer(const Color color, Point coord, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
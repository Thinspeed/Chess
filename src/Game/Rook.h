#pragma once
#include "Piece.h"
#include "GL/Program.h"

class Rook :
    public Piece
{
private:
public:
	bool WasMoved;
	Rook(const Color color, Point coord, float cellWidth, GL::Program *shader);
	Rook(const Color color, Point coord, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point coord) override;
};
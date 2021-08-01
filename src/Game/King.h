#pragma once
#include "Piece.h"
#include "GL/Program.h"

class King :
    public Piece
{
private:
public:
	bool WasMoved = false;
	King(const Color color, Point pos, float cellWidth, GL::Program *shader);
	King(const Color color, Point pos, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point pos) override;
};


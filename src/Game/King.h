#pragma once
#include "Piece.h"
class King :
    public Piece
{
private:
public:
	bool WasMoved = false;
	King(const Color color, Point pos, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point pos) override;
};


#pragma once
#include "Game/Piece.h"

class Pawn :
    public Piece
{
private:
	bool isFirstStep;
	bool twoCellStep;
public:
	Pawn(const Color color, Point pos, float cellWidth);
	bool MakeMove(Cell* map[8][8], Point pos) override;
	bool TryToKillPiece(Cell* map[8][8], Point pos) override;
};


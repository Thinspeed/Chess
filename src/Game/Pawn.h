#pragma once
#include "Game/Piece.h"
#include "GL/Program.h"

class Pawn :
    public Piece
{
private:
	bool isFirstStep;
	bool twoCellStep;
public:
	Pawn(const Color color, Point pos, float cellWidth, GL::Program *shader);
	Pawn(const Color color, Point pos, float cellWidth, GL::Model *model);
	bool MakeMove(Cell* map[8][8], Point pos) override;
	bool TryToKillPiece(Cell* map[8][8], Point pos) override;
};


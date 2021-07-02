#pragma once
#include "Piece.h"
#include "GL/VAO.h"
#include <iostream>
#include "Cell.h"

#define mapW 8

class Map
{
private:
	Cell* map[mapW][mapW];
	GLuint whiteSquareTexture;
	GLuint blackSquareTexture;
	GLuint chessTexture;
	Piece* pieceToRemove = nullptr;
	GLuint loadTexture(std::string path);
	void arrangePieces();
public:
	Point WhiteKingPos;
	Point BlackKingPos;
	float cellWidth;
	Piece* selectedPiece;
	Map(float width);
	void UnselectPiece();
	void SelectePiece(Color myColor, Point coord);
	bool TryToMovePiece(Point from, Point to);
	bool TryCastle(Color color, Point pos);
	bool IsCellUnderAttack(Point pos, Point from);
	bool IsKingUnderAttack(Color color);
	bool CanKillOrBlock(Color color, Point from);
	bool IsCheckMate(Color color, Point from);
	void Draw(GLuint modelMatrixID);
	~Map();
};


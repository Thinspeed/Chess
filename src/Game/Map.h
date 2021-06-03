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
	bool tryToKillPiece(Point to);
public:
	float cellWidth;
	Piece* selectedPiece;
	Map(float width);
	void UnselectPiece();
	void SelectePiece(Color myColor, Point coord);
	bool TryToMovePiece(Point from, Point to);
	void Draw(GLuint modelMatrixID);
	~Map();
};


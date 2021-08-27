#pragma once
#include "Piece.h"
#include "GL/VAO.h"
#include "GL/Program.h"
#include <iostream>
#include "Cell.h"
#include "GL/Texture.h"

#define mapW 8

class Map
{
private:
	Cell* map[mapW][mapW];
	Piece* pieceToRemove = nullptr;
	std::vector<GL::Model*> models;
	GL::Texture* whiteMarbleDiffuse;
	GL::Texture* whiteMarbleSpecular;
	GL::Texture* blackMarbleDiffuse;
	GL::Texture* blackMarbleSpecular;
	GL::Texture* lightWoodDiffuse;
	GL::Texture* lightWoodSpecular;
	GL::Texture* darkWoodDiffuse;
	GL::Texture* darkWoodSpecular;
	void loadTextures();
	void arrangePieces(GL::Program* shader);
public:
	Point WhiteKingPos;
	Point BlackKingPos;
	float cellWidth;
	Piece* selectedPiece;
	Map(float width, GL::Program *shader);
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


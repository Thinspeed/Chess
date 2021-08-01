#pragma once
#include "Piece.h"
#include "GL/Model.h"
#include "GL/Program.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Cell
{
private:
	float width;
	glm::mat4 modelMatrix;
public:
	Piece* mPiece;
	GL::Model *model_;
	Cell(Piece* piece, int x, int y, float cellWidth, GL::Program *shader);
	Cell(Piece* piece, int x, int y, float cellWidth, GL::Model *model);
	void Draw(GLuint modelMatrixID);
};


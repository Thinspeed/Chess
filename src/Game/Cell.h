#pragma once
#include "Piece.h"
#include "GL/VAO.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cell
{
private:
	float width;
public:
	Piece* mPiece;
	GL::VAO vao_;
	Cell(Piece* piece, int x, int y, float cellWidth);
	void Draw(GLuint modelMatrixID, GLuint cellTexture, GLuint chessTexture);
};


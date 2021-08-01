#include "Cell.h"

Cell::Cell(Piece* piece, int x, int y, float cellWidth, GL::Program *shader)
{
	mPiece = piece;
	width = cellWidth;
	std::string texturePath;
	if ((x + y) % 2 == 0)
	{
		texturePath = "textures/DarkWood";
	}
	else
	{
		texturePath = "textures/LightWood";
	}
	
	model_ = new GL::Model("models/other_models/cell.obj", texturePath, shader);
	modelMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(x * width, 0, -(y * width)));
}

Cell::Cell(Piece* piece, int x, int y, float cellWidth, GL::Model *model)
{
	mPiece = piece;
	width = cellWidth;
	std::string texturePath;
	model_ = model;
	modelMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(x * width, 0, -(y * width)));
}

void Cell::Draw(GLuint modelMatrixID)
{
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	model_->Draw();
	if (mPiece)
	{
		mPiece->Draw(modelMatrixID, width);
		
	}
}
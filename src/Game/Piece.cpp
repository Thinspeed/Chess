#include "Piece.h"
#include "GLFW/Window.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

bool Piece::MakeMove(Cell* map[8][8], Point pos)
{
	return false;
}

bool Piece::TryToKillPiece(Cell* map[8][8], Point pos)
{
	Piece* pieceToKill = map[pos.Y][pos.X]->mPiece;
	map[pos.Y][pos.X]->mPiece = nullptr;
	bool result = MakeMove(map, pos);
	map[pos.Y][pos.X]->mPiece = pieceToKill;
	return result;
}

void Piece::Draw(GLuint modelMatrixID, float cellWidth)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::mat4(1.0f);
	if (!isSelected)
	{
		modelMatrix = modelMatrix * glm::translate(glm::vec3(currentPos.X * cellWidth, 0, -(currentPos.Y * cellWidth)));
	}
	else
	{
		modelMatrix = modelMatrix * glm::translate(glm::vec3(currentPos.X * cellWidth, 0.25f, -(currentPos.Y * cellWidth)));
	}
	
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	model_->Draw();
}
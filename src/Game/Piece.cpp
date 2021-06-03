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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 model;
	model = glm::mat4(1.0f);
	if (!isSelected)
	{
		model = model * glm::translate(glm::vec3(currentPos.X * cellWidth, currentPos.Y * cellWidth, 0));
	}
	else
	{
		model = model * glm::translate(glm::vec3(currentPos.X * cellWidth, (currentPos.Y + 0.25f) * cellWidth, 3.0f));
	}
	
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);
	vao_.draw();
	glDisable(GL_BLEND);
}
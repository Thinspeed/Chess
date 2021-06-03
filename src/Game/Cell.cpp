#include "Cell.h"

Cell::Cell(Piece* piece, int x, int y, float cellWidth)
{
	mPiece = piece;
	width = cellWidth;
	vao_.addVertexBufferObject({
		{ x * width, y * width, -2 },
		{ x * width, (y + 1) * width, -2 },
		{ (x + 1) * width, (y + 1) * width, -2 },
		{ (x + 1) * width, y * width, -2 },
	});

	vao_.addVertexBufferObject({
		{ 0, 0 },
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 }
	});

	vao_.addIndices({ 0, 1, 2, 0, 2, 3 });
}

void Cell::Draw(GLuint modelMatrixID, GLuint cellTexture, GLuint chessTexture)
{
	glBindTexture(GL_TEXTURE_2D, cellTexture);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
	vao_.draw();
	if (mPiece)
	{
		glBindTexture(GL_TEXTURE_2D, chessTexture);
		mPiece->Draw(modelMatrixID, width);
		
	}
}
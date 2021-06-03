#include "Map.h"
#include "Pawn.h"
#include "Rook.h"
#include "Horse.h"
#include "Officer.h"
#include "Queen.h"
#include "King.h"
#include "Image/stb_image.h"
#include "GLFW/Window.h"
#include "GLFW/texture.hpp"

/**
 * \param width ������ ����� ������ � ������� �����������
 */
Map::Map(float width)
{
	cellWidth = width;
	arrangePieces();
	stbi_set_flip_vertically_on_load(false);
	whiteSquareTexture = loadTexture("textures/whiteSquareTexture.jpg");
	blackSquareTexture = loadTexture("textures/blackSquareTexture.jpg");
	chessTexture	   = texture_loadDDS("textures/chess.dds");
}

/**
 * \brief ��������� ��������
 * \param path ���� � �����
 */
GLuint Map::loadTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		throw std::runtime_error("Could not load texture: " + path);
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

/**
 * \brief ������ ������ � �������������� �� ����
 */
void Map::arrangePieces()
{
	struct Point coord;
	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapW; j++)
		{
			coord.X = j;
			coord.Y = i;
			map[i][j] = new Cell(nullptr, j, i, cellWidth);
		}
	}

	for (int i = 0; i < mapW; i++)
	{
		coord.X = i;
		coord.Y = 1;
		map[1][i]->mPiece = new Pawn(Color::White, coord, cellWidth);

		coord.X = i;
		coord.Y = 6;
		map[6][i]->mPiece = new Pawn(Color::Black, coord, cellWidth);
	}

	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 0 : 7;
			map[coord.Y][coord.X]->mPiece = new Rook(pColor, coord, cellWidth);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 1 : 6;
			map[coord.Y][coord.X]->mPiece = new Horse(pColor, coord, cellWidth);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 2 : 5;
			map[coord.Y][coord.X]->mPiece = new Officer(pColor, coord, cellWidth);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		coord.X = 4; coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		map[coord.Y][coord.X]->mPiece = new Queen(pColor, coord, cellWidth);
		coord.X = 3;
		map[coord.Y][coord.X]->mPiece = new King(pColor, coord, cellWidth);
	}
}

/**
 * \brief ���������� ������ �� ������� to, �� ������� ��������� selectedPiece.
 * ���������� true, ���� ���������� ����������� ������, false � ��������� ������
 * \param from ���������� ������, �� ������� ��������� ������
 * \param to ���������� ������, �� ������� ����� ����������� ������
 */
bool Map::TryToMovePiece(Point from, Point to)
{
	bool result = false;
	if ((selectedPiece != nullptr) && (from.X != to.X || from.Y != to.Y))
	{
		if (map[to.Y][to.X]->mPiece == nullptr)
		{
			result = selectedPiece->MakeMove(map, to);
		}
		else if (selectedPiece->pieceColor != map[to.Y][to.X]->mPiece->pieceColor)
		{
			result = selectedPiece->TryToKillPiece(map, to);
		}
	}

	if (result)
	{
		delete map[to.Y][to.X]->mPiece;
		map[to.Y][to.X]->mPiece = selectedPiece;
		selectedPiece->currentPos = to;
		map[from.Y][from.X]->mPiece = nullptr;
	}
	
	UnselectPiece();
	return result;
}

/**
 * \brief ������ ��� ��������� �����, ������� ������
 * \param modelMatrixID id ������� ������
 */
void Map::Draw(GLuint modelMatrixID)
{
	Point coord = Point(-1, -1);
	if (selectedPiece)
	{
		coord = selectedPiece->currentPos;
	}
	
	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapW; j++)
		{
			if (i != coord.Y || j != coord.X)
			{
				map[i][j]->Draw(modelMatrixID, (i + j) % 2 == 0 ? whiteSquareTexture : blackSquareTexture, chessTexture);
			}
		}
	}

	if (selectedPiece)
	{
		map[coord.Y][coord.X]->Draw(modelMatrixID, (coord.Y + coord.X) % 2 == 0 ? whiteSquareTexture : blackSquareTexture, chessTexture);
	}
}

/**
 * \brief ����������� ���� seectedPiece �������� null.
 * ���� �� ����� ���� selectedPiece �� ���� ����� null,
 * �� ���� isSelected �������������� false
 */
void Map::UnselectPiece()
{
	if (selectedPiece != nullptr)
	{
		selectedPiece->isSelected = false;
		selectedPiece = nullptr;
	}
}

/**
 * \brief ����������� ���� seectedCoord �������� Piece
 * �� ���� ������� map, ������� ����� ���������� coord
 */
void Map::SelectePiece(Color myColor, Point coord)
{
	if (selectedPiece)
	{
		selectedPiece->isSelected = false;
	}
	
	if (map[coord.Y][coord.X]->mPiece != nullptr && myColor == map[coord.Y][coord.X]->mPiece->pieceColor)
	{
		selectedPiece = map[coord.Y][coord.X]->mPiece;
		selectedPiece->isSelected = true;
	}
}

Map::~Map()
{
	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapW; j++)
		{
			delete(map[i][j]);
		}
	}
}

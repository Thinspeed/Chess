#include "Map.h"
#include "Pawn.h"
#include "Rook.h"
#include "Horse.h"
#include "Officer.h"
#include "Queen.h"
#include "King.h"
#include "GL/Model.h"
#include "Image/stb_image.h"
#include "GLFW/Window.h"
#include "GLFW/texture.hpp"

/**
 * \param width ширина одной клетки в мировых координатах
 */
Map::Map(float width, GL::Program* shader)
{
	cellWidth = width;
	arrangePieces(shader);
	WhiteKingPos = Point(3, 0);
	BlackKingPos = Point(3, 7);
	stbi_set_flip_vertically_on_load(false);
	whiteSquareTexture = loadTexture("textures/whiteSquareTexture.jpg");
	blackSquareTexture = loadTexture("textures/blackSquareTexture.jpg");
	chessTexture	   = texture_loadDDS("textures/chess.dds");
}

/**
 * \brief «агрузить текстуру
 * \param path ѕуть к файлу
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
 * \brief —оздаЄт фигуры и инициализурует их пол€
 */
void Map::arrangePieces(GL::Program* shader)
{
	struct Point coord;
	GL::Model *blackModel = new GL::Model("models/other_models/cell.obj", "textures/DarkWood", shader);
	GL::Model *whiteModel = new GL::Model("models/other_models/cell.obj", "textures/LightWood", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapW; j++)
		{ 
			coord.X = j;
			coord.Y = i;
			if ((i + j) % 2 == 0)
			{
				map[i][j] = new Cell(nullptr, j, i, cellWidth, whiteModel);
			}
			else
			{
				map[i][j] = new Cell(nullptr, j, i, cellWidth, blackModel);
			}
		}
	}
	
	blackModel = new GL::Model("models/chess_models/pawn.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/pawn.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);	
	for (int i = 0; i < mapW; i++)
	{
		coord.X = i;
		coord.Y = 1;
		map[1][i]->mPiece = new Pawn(Color::White, coord, cellWidth, whiteModel);

		coord.X = i;
		coord.Y = 6;
		map[6][i]->mPiece = new Pawn(Color::Black, coord, cellWidth, blackModel);
	}

	blackModel = new GL::Model("models/chess_models/rook.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/rook.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		GL::Model* model = i == 0 ? whiteModel : blackModel;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 0 : 7;
			map[coord.Y][coord.X]->mPiece = new Rook(pColor, coord, cellWidth, model);
		}
	}

	blackModel = new GL::Model("models/chess_models/horse.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/horse.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		GL::Model* model = i == 0 ? whiteModel : blackModel;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 1 : 6;
			map[coord.Y][coord.X]->mPiece = new Horse(pColor, coord, cellWidth, shader);
			map[coord.Y][coord.X]->mPiece = new Horse(pColor, coord, cellWidth, model);
		}
	}

	blackModel = new GL::Model("models/chess_models/bishop.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/bishop.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	for (int i = 0; i < 2; i++)
	{
		coord.Y = i == 0 ? 0 : 7;
		Color pColor = i == 0 ? Color::White : Color::Black;
		GL::Model* model = i == 0 ? whiteModel : blackModel;
		for (int j = 0; j < 2; j++)
		{
			coord.X = j == 0 ? 2 : 5;
			map[coord.Y][coord.X]->mPiece = new Officer(pColor, coord, cellWidth, shader);
			map[coord.Y][coord.X]->mPiece = new Officer(pColor, coord, cellWidth, model);
		}
	}

	blackModel = new GL::Model("models/chess_models/queen.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/queen.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	map[0][3]->mPiece = new Queen(Color::White, Point(3, 0), cellWidth, whiteModel);
	map[7][3]->mPiece = new Queen(Color::Black, Point(3, 7), cellWidth, blackModel);
	blackModel = new GL::Model("models/chess_models/king.obj", "textures/BlackMarble", shader);
	whiteModel = new GL::Model("models/chess_models/king.obj", "textures/WhiteMarble", shader);
	models.push_back(blackModel);
	models.push_back(whiteModel);
	map[0][4]->mPiece = new King(Color::White, Point(4, 0), cellWidth, whiteModel);
	map[7][4]->mPiece = new King(Color::Black, Point(4, 7), cellWidth, blackModel);
}

/**
 * \brief ѕеремещает фигуру на позицию to, на которую указывает selectedPiece.
 * ¬озвращает true, если получилось переместить фигуру, false в противном случае
 * \param from координаты клетка, на которой находитс€ фигура
 * \param to координаты клетки, на которую нужно передвинуть фигуру
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
		else if (from == WhiteKingPos || from == BlackKingPos)
		{
			return TryCastle(selectedPiece->pieceColor, to);
		}
	}
	
	UnselectPiece();
	if (result)
	{
		Piece* enemyPiece = map[to.Y][to.X]->mPiece;
		Piece* myPiece = map[from.Y][from.X]->mPiece;
		map[to.Y][to.X]->mPiece = myPiece;
		map[from.Y][from.X]->mPiece = nullptr;
		if (IsKingUnderAttack(myPiece->pieceColor))
		{
			map[to.Y][to.X]->mPiece = enemyPiece;
			map[from.Y][from.X]->mPiece = myPiece;
			return false;
		}

		delete enemyPiece;
		map[to.Y][to.X]->mPiece = myPiece;
		myPiece->currentPos = to;
		map[from.Y][from.X]->mPiece = nullptr;
		if (from.X == WhiteKingPos.X && from.Y == WhiteKingPos.Y)
		{
			WhiteKingPos = from;
		}
		else if (from.X == BlackKingPos.X && from.Y == BlackKingPos.Y)
		{
			BlackKingPos = from;
		}
	}
	
	return result;
}


bool Map::TryCastle(Color color, Point pos)
{
	King* king = dynamic_cast<King*>(selectedPiece);
	UnselectPiece();
	Rook* rook = dynamic_cast<Rook*>(map[pos.Y][pos.X]->mPiece);
	if (king == nullptr || king->WasMoved || IsKingUnderAttack(color))
	{
		return false;
	}

	if (color == Color::White)
	{
		WhiteKingPos = Point(1, pos.Y);
	}
	else
	{
		BlackKingPos = Point(1, pos.Y);
	}
	
	if (IsKingUnderAttack(color) || rook == nullptr || rook->WasMoved || !rook->MakeMove(map, Point(2, pos.Y)))
	{
		if (color == Color::White)
		{
			WhiteKingPos = Point(3, pos.Y);
		}
		else
		{
			BlackKingPos = Point(3, pos.Y);
		}
		
		return false;
	}
	
	map[pos.Y][1]->mPiece = map[pos.Y][3]->mPiece;
	map[pos.Y][3]->mPiece = nullptr;
	map[pos.Y][1]->mPiece->currentPos = Point(1, pos.Y);
	map[pos.Y][2]->mPiece = map[pos.Y][0]->mPiece;
	map[pos.Y][0]->mPiece = nullptr;
	map[pos.Y][2]->mPiece->currentPos = Point(2, pos.Y);
	return true;
}


/**
 * \brief ѕровер€ет находитс€ ли клетка под ударом
 * \param pos  летка, которую провер€ют
 * \param from  летка, где стоит фигура
 */
bool Map::IsCellUnderAttack(Point pos, Point from)
{
	if (map[from.Y][from.X] != nullptr && (from.X != pos.X || from.Y != pos.Y))
	{
		return map[from.Y][from.X]->mPiece->TryToKillPiece(map, pos);
	}

	return false;
}


bool Map::IsKingUnderAttack(Color color)
{
	Point kingPos = color == Color::White ? WhiteKingPos : BlackKingPos;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j]->mPiece != nullptr && map[i][j]->mPiece->pieceColor != color && IsCellUnderAttack(kingPos, Point(j, i)))
			{
				return true;
			}
		}
	}
	
	return false;
}


bool Map::IsCheckMate(Color color, Point from)
{
	Point kingPos = color == Color::White ? WhiteKingPos : BlackKingPos;
	for (int dx = -1; dx < 2; dx++)
	{
		for (int dy = -1; dy < 2; dy++)
		{
			if ((kingPos.X + dx < 8) && (kingPos.X + dx > -1) && (kingPos.Y + dy < 8) && (kingPos.Y + dy > -1))
			{
				if (map[kingPos.Y][kingPos.X]->mPiece->TryToKillPiece(map, Point(kingPos.Y + dy, kingPos.X + dx)))
				{
					return false;
				}
			}
		}
	}

	std::vector<Point> posToBlock;
	int deltaX = from.X - kingPos.X;
	deltaX = deltaX == 0 ? 0 : deltaX / abs(deltaX);
	int deltaY = from.Y - kingPos.Y;
	deltaY = deltaY == 0 ? 0 : deltaY / abs(deltaY);
	Point pos(kingPos.X, kingPos.Y);
	do
	{
		pos.X += deltaX;
		pos.Y += deltaY;
		posToBlock.push_back(pos);
	}
	while (pos != from);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (map[i][j]->mPiece != nullptr && map[i][j]->mPiece->pieceColor == color)
			{
				for (int k = 0; k < posToBlock.size(); k++)
				{
					if (map[i][j]->mPiece->TryToKillPiece(map, posToBlock[k]))
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}


/**
 * \brief –исует всю шахматную доску, включа€ фигуры
 * \param modelMatrixID id матрицы модели
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
				map[i][j]->Draw(modelMatrixID);
			}
		}
	}

	if (selectedPiece)
	{
		map[coord.Y][coord.X]->Draw(modelMatrixID);
	}
}

/**
 * \brief ѕрисваивает полю seectedPiece значение null.
 * ≈сли до этого поле selectedPiece не было равно null,
 * то поле isSelected приравниваетс€ false
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
 * \brief ѕрисваивает полю seectedCoord значение Piece
 * из пол€ массива map, которое имеет координаты coord
 */
void Map::SelectePiece(Color myColor, Point coord)
{
	if (selectedPiece)
	{
		selectedPiece->isSelected = false;
		selectedPiece = nullptr;
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

	for (int i = 0; i < models.size(); i++)
	{
		delete(models[i]);
	}
}

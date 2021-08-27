#pragma once
#include "Map.h"
#include "GL/Scene.h"
#include "Net/Net.h"

enum class Code
{
	BrokenConnection = -1,
	Color = 0,
	PieceMove = 1,
	EndOfGame = 2,
};

class Game
	: public Scene
{
private:
	std::thread netThread;
	Net* net;
	Point kingPos;
	bool isKingUnderAttack = false;
	void sendGameInfo();
	void receiveGameInfo();
	void finishMove(Point from, Point to);
	void waitForMove();
public:
	Color myColor;
	bool IsConnected = false;
	bool IsMyTurn = false;
	bool IsGameFinished = false;
	Map* chessMap;
	Game(GL::Program* shader, Text* textPrinter);
	Game(std::string ip, GL::Program* shader, Text* textPrinter);
	void ProcessKeyboardInput(int key) override;
	void ProcessMouseInput(std::vector<glm::vec3> ray) override;
	void FinishGame();
	void Draw(GLuint ModelMatrixID) override;
	~Game();
};


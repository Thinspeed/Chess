#pragma once
#include "Map.h"
#include "Net/Net.h"

enum class Code
{
	Color = 0,
	PieceMove = 1,
	EndOfGame = 2,
};

class Game
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
	Game();
	Game(std::string ip);
	void ProcessMapInput(std::vector<glm::vec3> ray);
	void FinishGame();
	~Game();
};


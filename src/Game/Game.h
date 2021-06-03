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
	void sendGameInfo();
	void receiveGameInfo();
	void finishMove(Point from, Point to);
	void waitForMove();
public:
	Color myColor;
	bool IsConnected = false;
	bool IsMyTurn = false;
	Map* chessMap;
	Game();
	Game(std::string ip);
	void ProcessMapInput(float xPos, float yPos);
};


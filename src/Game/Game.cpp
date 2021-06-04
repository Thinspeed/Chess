#include "Game.h"
#include "Net/Server.h"
#include "Net/Client.h"
#include <thread>

Game::Game()
{
	net = (Net*)(new Server);
	netThread = std::thread([this] { ((Server*)net)->Listen(); sendGameInfo(); });
}

Game::Game(std::string ip)
{
	net = (Net*)(new Client);
	netThread = std::thread([this, ip] { ((Client*)net)->Connect(ip); receiveGameInfo(); });
}

void Game::sendGameInfo()
{
	myColor = (Color)((rand() % 2 == 0) ? 1 : -1);
	IsMyTurn = (int)myColor + 1;
	IsConnected = true;
	std::cout << ((int)myColor == 1 ? "White" : "Black");
	int buf[2] = { (int)Code::Color, (int)myColor * -1 };
	net->sendData(buf, 2);
	if (!IsMyTurn)
	{
		sendRecvThread = std::thread([this] { waitForMove(); });
		sendRecvThread.detach();
	}
}

void Game::receiveGameInfo()
{
	int *buf = net->receiveData();
	if ((Code)buf[0] == Code::Color)
	{
		myColor = (Color)buf[1];
		IsMyTurn = (int)myColor + 1;
		IsConnected = true;
		std::cout << ((int)myColor == 1 ? "White" : "Black");
	}
	else
	{
		throw std::runtime_error("Check connection with server");
	}
	
	if (!IsMyTurn)
	{
		sendRecvThread = std::thread([this] { waitForMove(); });
		sendRecvThread.detach();
	}
}

void Game::ProcessMapInput(float xPos, float yPos)
{
	int i = yPos / chessMap->cellWidth;
	int j = xPos / chessMap->cellWidth;

	if (i < 0 || i > 7 || j < 0 || j > 7)
	{
		return;
	}
	
	if (chessMap->selectedPiece)
	{
		Point from = chessMap->selectedPiece->currentPos;
		IsMyTurn = !chessMap->TryToMovePiece(from, Point(j, i));
		if (!IsMyTurn)
		{
			finishMove(from, Point(j, i));
			sendRecvThread = std::thread([this] { waitForMove(); });
		}
	}
	else
	{
		chessMap->SelectePiece(myColor, Point(j, i));
	}
}

void Game::finishMove(Point from, Point to)
{
	int *buf = (int*)malloc(sizeof(int) * 5);
	buf[0] = (int)Code::PieceMove; buf[1] = from.X; buf[2] = from.Y; buf[3] = to.X; buf[4] = to.Y;
	//auto a = [](Game* game, int* buf) -> void { game->net->sendData(buf, 5); game->waitForMove(); };
	net->sendData(buf, 5);
	IsMyTurn = false;
}

void Game::waitForMove()
{
	int* buf;
	buf = net->receiveData();
	if ((Code)buf[0] == Code::PieceMove)
	{
		chessMap->SelectePiece(Color((int)myColor * -1), Point(buf[1], buf[2]));
		if (!chessMap->TryToMovePiece(Point(buf[1], buf[2]), Point(buf[3], buf[4])))
		{
			throw std::runtime_error("Mistake in received data");
		}
	}
	
	IsMyTurn = true;
}
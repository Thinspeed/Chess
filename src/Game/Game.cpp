#include "Game.h"
#include "Net/Server.h"
#include "Net/Client.h"
#include <thread>

Game::Game()
{
	net = (Net*)(new Server);
	netThread = std::thread([this] { ((Server*)net)->Listen(); sendGameInfo(); waitForMove(); });
}

Game::Game(std::string ip)
{
	net = (Net*)(new Client);
	netThread = std::thread([this, ip] { ((Client*)net)->Connect(ip); receiveGameInfo(); waitForMove();  });
}

void Game::sendGameInfo()
{
	srand(time(nullptr));
	//myColor = (Color)((rand() % 2 == 0) ? 1 : -1);
	myColor = Color::Black;
	IsMyTurn = (int)myColor + 1;
	kingPos = Point(3, myColor == Color::White ? 0 : 7);
	IsConnected = true;
	std::cout << ((int)myColor == 1 ? "White" : "Black\n");
	int buf[2] = { (int)Code::Color, (int)myColor * -1 };
	net->sendData(buf, 2);
}

void Game::receiveGameInfo()
{
	int *buf = net->receiveData();
	if ((Code)buf[0] == Code::Color)
	{
		myColor = (Color)buf[1];
		IsMyTurn = (int)myColor + 1;
		kingPos = Point(3, myColor == Color::White ? 0 : 7);
		IsConnected = true;
		std::cout << ((int)myColor == 1 ? "White" : "Black\n");
	}
	else
	{
		throw std::runtime_error("Check connection with server");
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
			if (from.X == kingPos.X && from.Y == kingPos.Y)
			{
				kingPos.X = abs(kingPos.X - j) == 1 ? j : 1;
				kingPos.Y = i;
			}
			
			finishMove(from, Point(j, i));
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
	while (!IsGameFinished)
	{
		int* buf;
		buf = net->receiveData();
		if ((Code)buf[0] == Code::PieceMove)
		{
			Point from = Point(buf[1], buf[2]);
			Point to = Point(buf[3], buf[4]);
			chessMap->SelectePiece(Color((int)myColor * -1), from);
			
			if (!chessMap->TryToMovePiece(from, to))
			{
				throw std::runtime_error("Received wrong data, could not move piece that way");
			}

			isKingUnderAttack = chessMap->IsKingUnderAttack(myColor);
			if (isKingUnderAttack && chessMap->IsCheckMate(myColor, to))
			{
				IsGameFinished = true;
			}
		}
		else if ((Code)buf[0] == Code::EndOfGame)
		{
			IsGameFinished = true;
		}
		else
		{
			throw std::runtime_error("Wrong code in data, code: " + buf[0]);
		}
		
		IsMyTurn = true;
	}

	FinishGame();
}

void Game::FinishGame()
{
	int* buf = (int*)malloc(sizeof(int));
	buf[0] = (int)Code::EndOfGame;
	IsGameFinished = true;
	net->sendData(buf, 1);
}

Game::~Game()
{
	netThread.join();
	delete chessMap;
	if (net != nullptr)
	{
		net->closeConnection();
		delete net;
		net = nullptr;
	}
}
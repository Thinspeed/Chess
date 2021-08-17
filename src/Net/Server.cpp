#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <iostream>
#include <mutex>

Server::Server()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		throw std::runtime_error("WSAStartup failed");
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(5663);
	sin.sin_addr.s_addr = inet_addr(getIp());
	
	initSocket();
	bindSoket();
}

char* Server::getIp()
{
	char hn[1024];
	struct hostent* adr;
	if (gethostname(hn, strlen(hn)))
	{
		return NULL;
	}

	adr = gethostbyname(hn);
	adr->h_addr_list;
	int nAdpter = 0;
	while (adr->h_addr_list[nAdpter + 1])
	{
		nAdpter++;
	}
	
	char* LocalIp = (char*)malloc(sizeof(char) * 128);
	sprintf_s(LocalIp, 128, "%d.%d.%d.%d",
		(unsigned char)adr->h_addr_list[nAdpter][0],
		(unsigned char)adr->h_addr_list[nAdpter][1],
		(unsigned char)adr->h_addr_list[nAdpter][2],
		(unsigned char)adr->h_addr_list[nAdpter][3]);
	printf_s("%s\n", LocalIp);

	return LocalIp;
}

void Server::initSocket()
{
	socket_ = socket(AF_INET, SOCK_STREAM, NULL);
	if (socket_ == INVALID_SOCKET || socket_ == SOCKET_ERROR)
	{
		throw std::runtime_error("Creating socket failed");
	}
}

void Server::bindSoket()
{
	if (bind(socket_, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		throw std::runtime_error("Socket binding failed");
	}
}

void Server::Listen()
{
	if (listen(socket_, 1) == SOCKET_ERROR)
	{
		throw std::runtime_error("listen failed with error: " + WSAGetLastError());
	}

	int len = sizeof(sin);
	client = accept(socket_, NULL, NULL);
	if (client == INVALID_SOCKET)
	{
		throw std::runtime_error("Accept failed: " + WSAGetLastError());
	}
	std::cout << "Someone connected\n";
}

bool Server::sendData(int* buf, int length)
{
	if (send(client, (char*)buf, length * sizeof(int) / sizeof(char), 0) != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int* Server::receiveData()
{
	const int max_client_buffer_size = 64;
	char buf[max_client_buffer_size];

	int bytes = recv(client, buf, max_client_buffer_size - 1, 0);
	if (bytes == SOCKET_ERROR) {
		closesocket(client);
		int* temp = (int*)buf;
		*temp = SOCKET_ERROR; *(temp + 1) = '\0';
	}
	
	return (int*)buf;
}

void Server::closeConnection()
{
	closesocket(client);
	closesocket(socket_);
}

Server::~Server()
{
	closesocket(client);
	closesocket(socket_);
	WSACleanup();
}

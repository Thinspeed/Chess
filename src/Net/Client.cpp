#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"
#include <iostream>
#include <mutex>
#include <sys/types.h>
#include <winsock2.h>

std::mutex a;

Client::Client()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		throw std::runtime_error("WSAStartup failed");
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(5663);

	initSocket();
}

void Client::initSocket()
{
	socket_ = socket(AF_INET, SOCK_STREAM, NULL);
	if (socket_ == INVALID_SOCKET || socket_ == SOCKET_ERROR)
	{
		throw std::runtime_error("Creating socket failed");
	}
}

void Client::Connect(std::string ip)
{
	sin.sin_addr.s_addr = inet_addr(ip.c_str());
	int res = connect(socket_, (SOCKADDR*)&sin, sizeof(sin));
	if (res == -1)
	{
		wchar_t* s = NULL;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&s, 0, NULL);
		fprintf(stderr, "%S\n", s);
		LocalFree(s);
		throw std::runtime_error("Connection failed ");
	}

	std::string message;
	send(socket_, message.c_str(), message.length(), 0);
}

bool Client::sendData(int* buf, int length)
{

	
	if (send(socket_, (char*)buf, length * sizeof(int) / sizeof(char), 0) != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int* Client::receiveData()
{
	const int max_client_buffer_size = 64;
	char buf[max_client_buffer_size];

	try
	{
		a.lock();
		int bytes = recv(socket_, buf, max_client_buffer_size - 1, 0);
		if (bytes == SOCKET_ERROR) {
			closesocket(socket_);
			throw std::runtime_error("receive failed: ");
		}
		a.unlock();
	}
	catch (const std::exception& error)
	{
		wchar_t* s = NULL;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&s, 0, NULL);
		fprintf(stderr, "%S\n", s);
		LocalFree(s);
		throw error;
	}
	
	return (int*)buf;
}

Client::~Client()
{
}

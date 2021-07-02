#pragma once
#include "Net.h"
#include <thread>
#include <sys/types.h>
#include <winsock2.h>

class Server : Net
{
private:
	SOCKET client;
	char* getIp();
	void initSocket();
	void bindSoket();
	void exchangeData();
public:
	Server();
	void Listen();
	bool sendData(int* buf, int length) override;
	int* receiveData() override;
	void closeConnection() override;
	~Server();
};


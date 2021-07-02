#pragma once
#include "Net.h"

class Client : Net
{
private:
	void initSocket();	
public:
	Client();
	void Connect(std::string ip);
	bool sendData(int* buf, int length) override;
	int* receiveData() override;
	void closeConnection() override;
	~Client();
};


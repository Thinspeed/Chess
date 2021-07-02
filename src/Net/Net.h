#pragma once
#include <thread>
#include <sys/types.h>
#include <winsock2.h>

class Net
{
protected:
	SOCKADDR_IN sin;
	SOCKET socket_;
	WSADATA wsaData;
public:
	virtual bool sendData(int* buf, int length);
	virtual int* receiveData();
	virtual void closeConnection();
};


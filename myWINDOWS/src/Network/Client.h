#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib") 
#include<WinSock2.h>
#include <string.h>
#include<iostream>
#include <functional>

#define ASSERT __debugbreak()
#define C(x) std::cout<<x<<std::endl;

class Client
{
public:
	Client();

	void listenConnection();  // thread
	int listen();
	int send(std::string data);

	void setRecieveCallback(std::function<void(std::string)> func);
private:
	int port = 1234;
	const char* ip = "140.116.133.7";
	SOCKADDR_IN addr;
	SOCKET sConnect;
	char buffer[1024];
	std::function<void(std::string)> recvCallback = nullptr;
};
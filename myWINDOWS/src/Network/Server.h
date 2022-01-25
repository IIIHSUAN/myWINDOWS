#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib") 
#include<WinSock2.h>
#include <string.h>
#include<iostream>
#include <functional>

#define ASSERT __debugbreak()
#define C(x) std::cout<<x<<std::endl;

class Server
{
public:
	Server();
	~Server();

	void listenConnection();  // thread
	int send(std::string data);
	int listen();

	void setRecieveCallback(std::function<void(std::string)> func);
private:
	int port = 1234;
	const char* ip = "140.116.133.7";
	char buffer[1024];
	SOCKET sListen, sConnect;
	struct sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	std::function<void(std::string)> recvCallback = nullptr;
};
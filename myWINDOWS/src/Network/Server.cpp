#include "Server.h"

Server::Server()
{
	WSAData wsaData;
	WORD dllVersion = MAKEWORD(2, 1);  //network version
	if (WSAStartup(dllVersion, &wsaData) != 0)  //initial
		return;
	else
		C("initial network");

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 1)
	{
		ASSERT, C("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return;
	}
	else
		C("Winsock 2.1 dll found\n");

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		ASSERT, C("Connection Invalid");
		return;
	}
	else
		C("Setting socket");

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(ip);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);  //host to net short
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)  //addr from SOCKADDR_IN to SOCKADDR
		return;

	if (::listen(sListen, SOMAXCONN) == SOCKET_ERROR)  //max can listen size
		return;
	else
		C("listen from socket is set, and start the client's connectionOriented(SOCK_STREAM)\n");
}

Server::~Server()
{
	closesocket(sConnect);
	WSACleanup();
}

void Server::listenConnection()
{
	C("listen...");
	sConnect = accept(sListen, (SOCKADDR*)&clientAddr, &clientAddrLen);  //IP can be null
																		 // sConnect = accept(sListen, NULL, NULL);
	if (sConnect == INVALID_SOCKET)
	{
		WSAGetLastError();
		return;
	}

	send("hi");

	listen();  // start listening
}

int Server::send(std::string data)
{
	if (::send(sConnect, data.c_str(), data.length(), 0) == SOCKET_ERROR)
	{
		C("send failed");
		return 1;
	}
	return 0;
}

int Server::listen()
{
	while (true)
	{
		memset(&buffer, 0, sizeof(buffer));
		if (recv(sConnect, buffer, sizeof(buffer), 0) == -1)
			return 1;

		C("recv " << buffer);

		if (recieveCallback)
			recieveCallback(buffer);
	}

	return 0;
}

void Server::setRecieveCallback(std::function<void(std::string)> func)
{
	recieveCallback = std::bind(func, std::placeholders::_1);
}

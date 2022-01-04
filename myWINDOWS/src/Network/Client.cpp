#include "Client.h"

Client::Client()
{
	WSAData wsaData;
	WORD dllVersion = MAKEWORD(2, 1);  //network version
	if (WSAStartup(dllVersion, &wsaData) != 0)  //initial
	{
		ASSERT, C("Network setup failed");
		return;
	}
	else
		C("initial network");

	
	memset(&addr, 0, sizeof(addr));  //clear data
	addr.sin_addr.s_addr = inet_addr(ip);  //= INADDR_ANY:for any IP
										   //inet_addr now is NetworkByteOrder (contrary to HostByteOrder)
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	sConnect = socket(AF_INET, SOCK_STREAM, NULL);
}

void Client::listenConnection()
{
	while (1)
	{
		if (connect(sConnect, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			WSAGetLastError();
			C("disconnect");
		}
		else
		{
			C("connected");
			break;
		}

		Sleep(1000);
	}

	listen();  // start listening
}

int Client::listen()
{
	while (true)
	{
		memset(&buffer, 0, sizeof(buffer));
		if (recv(sConnect, buffer, sizeof(buffer), 0) == -1)
		{
			C("recieve failed");
			return 1;
		}

		C("recv " << buffer);

		if (recieveCallback)
			recieveCallback(buffer);
	}

	return 0;
}

int Client::send(std::string data)
{
	if (::send(sConnect, data.c_str(), data.length(), 0) == SOCKET_ERROR)
	{
		C("send failed");
		return 1;
	}
	return 0;
}

void Client::setRecieveCallback(std::function<void(std::string)> func)
{
	recieveCallback = std::bind(func, std::placeholders::_1);
}

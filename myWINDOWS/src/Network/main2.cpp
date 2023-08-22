#include <iostream>
#include <thread>
#include "Client.h"

void r(std::string data)
{

	std::cout << "AAAAAAAA " << data.c_str();
}

int refmain()
{
	Client client;

	client.setRecieveCallback(r);
	
	std::thread net([&client]() {
		client.listenConnection();
	});
	net.detach();
	while (1)
	{
		std::cout << "main\n";
		Sleep(1000);
	}

	return 0;
}
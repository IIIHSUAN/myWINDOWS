#include <iostream>
#include <thread>
#include "Server.h"

int refmainmain()
{
	Server server;

	std::thread net([&server]() {
		server.listenConnection();
	});
	net.detach();
	while (1)
	{
		std::cout << "main\n";
		Sleep(1000);
	}

	return 0;
}
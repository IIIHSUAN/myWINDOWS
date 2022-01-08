#pragma execution_character_set("utf-8")

#include "AppHandler/AppHandler.h"


int main(int argc, char** args)
{

	// entry point
	auto& appHandler = AppHandler::get();
	appHandler.run();

	return 0;
}
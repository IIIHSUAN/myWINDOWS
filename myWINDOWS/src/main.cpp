#include "AppHandler/AppHandler.h"

int main(int argc, char** args)
{
	// entry point;
	auto& appHandler = AppHandler::get();
	appHandler.run();

	return 0;
}
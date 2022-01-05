#include "AppHandler/AppHandler.h"

int main(int argc, char** args)
{
	Output::get().setFontSize(20);

	// entry point
	auto& appHandler = AppHandler::get();
	appHandler.run();

	return 0;
}
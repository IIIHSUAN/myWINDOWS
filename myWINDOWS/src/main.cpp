#include "System/System.h"

int main(int argc, char** args)
{

	// entry point
	auto& appHandler = System::get();
	appHandler.run();

	return 0;
}
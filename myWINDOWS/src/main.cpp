#include "Window/WindowHandler.h"

int main(int argc, char** args)
{
	// entry point;
	auto& windowHandler = WindowHandler::get();
	windowHandler.run();

	return 0;

	Game* game = new Game();

	game->run();

	game->distroy();
	return 0;
}
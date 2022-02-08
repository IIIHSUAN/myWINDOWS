#include "Chess.h"

ChessWindow::ChessWindow(std::wstring _name, Pos _pos, Size _size) : Window(_name, _pos, _size)
{
	getCanvas().line(60, 5, std::wstring(L"Game Starts"));
}

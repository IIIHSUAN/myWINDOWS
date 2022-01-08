#include "Chess.h"

ChessWindow::ChessWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size)
{
	getCanvas().line(60, 5, std::wstring(L"Game Starts"));
}

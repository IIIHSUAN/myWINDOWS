#include "IO.h"

#include "AppHandler.h"

/* Input *******************************************************/

Mouse* Mouse::mouse = new Mouse();
Input* Input::input = new Input();

Input::Input()
{
	PeekConsoleInput(inHandle, &buffer, 1, &events);
}

void Input::run()
{
	INPUT_RECORD InputRecord;
	SetConsoleMode(inHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);
	bool isFlush = false;
	static AppHandler& appHandler = AppHandler::get();
	while (appHandler.isRun)
	{
		isFlush = false;

		ReadConsoleInput(inHandle, &InputRecord, 1, &num);
		switch (InputRecord.EventType) {
		case KEY_EVENT:
			isFlush |= appHandler.keyEvent(InputRecord.Event.KeyEvent.wVirtualKeyCode, InputRecord.Event.KeyEvent.dwControlKeyState, InputRecord.Event.KeyEvent.bKeyDown);
			break;
		}

		if (isFlush)
			appHandler.update();
	}
}

/* Output *******************************************************/

Output* Output::output = new Output();

Output::Output()
{
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(outHandle, FALSE, &cfi);
}

void Output::display(const wchar_t* pixels)
{
	system("cls");

	//SetConsoleTextAttribute(outHandle, 100);  //0~254

	for (int i = 0; i < MY_WINDOW_HEIGHT; i++)
	{
		WriteConsoleW(outHandle, pixels + i * MY_WINDOW_WIDTH, MY_WINDOW_WIDTH, &num, NULL);
		wprintf(L"\n");
	}
}

void Output::setFontSize(int size)
{
	cfi.dwFontSize.Y = size;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

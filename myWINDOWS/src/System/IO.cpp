#include "IO.h"

#include <ctime>

#include "System.h"
#include "App/App.h"
#include "Window/Event.h"

/* Input *******************************************************/

MyMouse* MyMouse::mouse = new MyMouse();
Input* Input::input = new Input();

Input::Input()
{
	for (auto& m : pollingMap)
		m = false;
}

void Input::run()
{
	INPUT_RECORD inputBuffer[128];
	resetInputSettings();
	
	while (System::get().isRun)
	{
		ReadConsoleInput(inHandle, inputBuffer, 128, &num);

		for (int i = 0; i < int(num); i++)
		{
			switch (inputBuffer[i].EventType) {
			case KEY_EVENT:
			{
				KEY_EVENT_RECORD& keyEvent = inputBuffer[i].Event.KeyEvent; keyEvent.dwControlKeyState;
				if (keyEvent.bKeyDown)
					System::get().onKeyPrsEvent(keyEvent.wVirtualKeyCode, keyEvent.uChar.UnicodeChar);

				keyMapping(keyEvent.wVirtualKeyCode, keyEvent.bKeyDown);
				break;
			}
			case MOUSE_EVENT:
			{
				MOUSE_EVENT_RECORD& mouseEvent = inputBuffer[i].Event.MouseEvent;
				switch (mouseEvent.dwEventFlags)
				{
				case 0:
				{
					PollingInput in = mouseMapping(mouseEvent.dwButtonState);
					if (in != PollingInput::unknown)
						System::get().onMousePrsEvent(in);
					else
						System::get().onMouseRlsEvent();
					break;
				}
				case MOUSE_MOVED:
				{
					System::get().onMouseMoveEvent(mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
					break;
				}
				}
				break;
			}
			case WINDOW_BUFFER_SIZE_EVENT:  // spanning
			{
				WINDOW_BUFFER_SIZE_RECORD& resizeEvent = inputBuffer[i].Event.WindowBufferSizeEvent;
				
				if (resize.X != resizeEvent.dwSize.X || resize.Y != resizeEvent.dwSize.Y)
				{
					resize.X = resizeEvent.dwSize.X, resize.Y = resizeEvent.dwSize.Y;
					resizeCount = 5;

					if (!isResizeThread)
					{
						isResizeThread = true;
						std::thread resizeThread = std::thread([this]() {
							while(--resizeCount) Sleep(100);
							System::get().onResizeEvent(resize.X, resize.Y), isResizeThread = false;
						});
						resizeThread.detach();
					}
				}
				break;
			}
			}
		}
	}
}

void Input::keyMapping(WORD& key,bool isPrs)
{
#define KEY_CASE_POLLING_MAP(x) case KeySet::x: if(pollingMap[UINT(PollingInput::x)]!=isPrs) pollingMap[UINT(PollingInput::x)] = isPrs; return
	switch (KeyPrsEvent::toKeySet(key))
	{
		KEY_CASE_POLLING_MAP(shift);
		KEY_CASE_POLLING_MAP(ctrl);
		KEY_CASE_POLLING_MAP(spacebar);
		KEY_CASE_POLLING_MAP(enter);
		KEY_CASE_POLLING_MAP(up);
		KEY_CASE_POLLING_MAP(down);
		KEY_CASE_POLLING_MAP(left);
		KEY_CASE_POLLING_MAP(right);
		KEY_CASE_POLLING_MAP(A);
		KEY_CASE_POLLING_MAP(B);
		KEY_CASE_POLLING_MAP(C);
		KEY_CASE_POLLING_MAP(D);
		KEY_CASE_POLLING_MAP(E);
		KEY_CASE_POLLING_MAP(F);
		KEY_CASE_POLLING_MAP(G);
		KEY_CASE_POLLING_MAP(H);
		KEY_CASE_POLLING_MAP(I);
		KEY_CASE_POLLING_MAP(J);
		KEY_CASE_POLLING_MAP(K);
		KEY_CASE_POLLING_MAP(L);
		KEY_CASE_POLLING_MAP(M);
		KEY_CASE_POLLING_MAP(N);
		KEY_CASE_POLLING_MAP(O);
		KEY_CASE_POLLING_MAP(P);
		KEY_CASE_POLLING_MAP(Q);
		KEY_CASE_POLLING_MAP(R);
		KEY_CASE_POLLING_MAP(S);
		KEY_CASE_POLLING_MAP(T);
		KEY_CASE_POLLING_MAP(U);
		KEY_CASE_POLLING_MAP(V);
		KEY_CASE_POLLING_MAP(W);
		KEY_CASE_POLLING_MAP(X);
		KEY_CASE_POLLING_MAP(Y);
		KEY_CASE_POLLING_MAP(Z);
		KEY_CASE_POLLING_MAP(num0);
		KEY_CASE_POLLING_MAP(num1);
		KEY_CASE_POLLING_MAP(num2);
		KEY_CASE_POLLING_MAP(num3);
		KEY_CASE_POLLING_MAP(num4);
		KEY_CASE_POLLING_MAP(num5);
		KEY_CASE_POLLING_MAP(num6);
		KEY_CASE_POLLING_MAP(num7);
		KEY_CASE_POLLING_MAP(num8);
		KEY_CASE_POLLING_MAP(num9);
		KEY_CASE_POLLING_MAP(pad0);
		KEY_CASE_POLLING_MAP(pad1);
		KEY_CASE_POLLING_MAP(pad2);
		KEY_CASE_POLLING_MAP(pad3);
		KEY_CASE_POLLING_MAP(pad4);
		KEY_CASE_POLLING_MAP(pad5);
		KEY_CASE_POLLING_MAP(pad6);
		KEY_CASE_POLLING_MAP(pad7);
		KEY_CASE_POLLING_MAP(pad8);
		KEY_CASE_POLLING_MAP(pad9);
	default: return;
	}
}

PollingInput Input::mouseMapping(DWORD& state)
{
#define MOUSE_KEY_CASE_POLLING_MAP(originCase, x) \
case originCase: lastMouseInput = PollingInput::x, pollingMap[UINT(lastMouseInput)] = true; return PollingInput::x

	static PollingInput lastMouseInput = PollingInput::unknown;
	switch (state)
	{
		MOUSE_KEY_CASE_POLLING_MAP(FROM_LEFT_1ST_BUTTON_PRESSED, mouse_l1);
		MOUSE_KEY_CASE_POLLING_MAP(FROM_LEFT_2ND_BUTTON_PRESSED, mouse_l2);
		MOUSE_KEY_CASE_POLLING_MAP(FROM_LEFT_3RD_BUTTON_PRESSED, mouse_l3);
		MOUSE_KEY_CASE_POLLING_MAP(FROM_LEFT_4TH_BUTTON_PRESSED, mouse_l4);
		MOUSE_KEY_CASE_POLLING_MAP(RIGHTMOST_BUTTON_PRESSED, mouse_r1);
	default: pollingMap[UINT(lastMouseInput)] = false; return PollingInput::unknown;
	}
}

void Input::resetInputSettings()
{
	SetConsoleMode(inHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | DISABLE_NEWLINE_AUTO_RETURN);
}

/* Output *******************************************************/

Output* Output::output = new Output();

short Output::width = 200;
short Output::height = 50;

Output::Output()
{
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontsizeX;
	cfi.dwFontSize.Y = fontsizeY;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"MS Gothic");
	SetCurrentConsoleFontEx(outHandle, FALSE, &cfi);

	SetConsoleScreenBufferSize(outHandle, { width,height });
	system(std::string("mode " + std::to_string(width) + ',' + std::to_string(height)).c_str());

	HWND console = GetConsoleWindow();
	ShowScrollBar(console, SB_BOTH, FALSE);
	SetConsoleTitle(TEXT("C++ Browser Engine"));
}

void Output::display(const std::wstring& pixels)
{
	//SetConsoleTextAttribute(outHandle, 100);  //0~254

	WriteConsoleW(outHandle, pixels.c_str(), DWORD(pixels.length()), &num, NULL);
	return;

	//for (int i = 0; i < height; i++)
	//	WriteConsoleW(outHandle, pixels + i * width, width, &num, NULL), wprintf(L"\n");
}

void Output::setFontSize(int _fontsizeY)
{
	fontsizeY = _fontsizeY;
	cfi.dwFontSize.X = fontsizeX;
	cfi.dwFontSize.Y = fontsizeY;
	SetCurrentConsoleFontEx(outHandle, FALSE, &cfi);
}

void Output::onResizeEvent(ResizeEvent& e)
{
	MY_WINDOW_WIDTH = e.getWidth(), MY_WINDOW_HEIGHT = e.getHeight();

	SetConsoleScreenBufferSize(outHandle, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT });
	system(std::string("mode " + std::to_string(MY_WINDOW_WIDTH) + ',' + std::to_string(MY_WINDOW_HEIGHT)).c_str());

	Input::get().resetInputSettings();
}
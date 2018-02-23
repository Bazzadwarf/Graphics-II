#include "DXWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DXWindow * _thisWindow = NULL;

DXWindow::DXWindow()
{
}

DXWindow::DXWindow(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow)
{
	_thisWindow = this;
	_hInstance = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;
	_width = DEFAULT_WIDTH;
	_height = DEFAULT_HEIGHT;
}


DXWindow::~DXWindow()
{
}

void DXWindow::SetUpWindow()
{
	// clear out the window class for use
	ZeroMemory(&_wcex, sizeof(WNDCLASSEXW));

	// fill in the struct with the needed information
	_wcex.cbSize = sizeof(WNDCLASSEXW);
	_wcex.style = CS_HREDRAW | CS_VREDRAW;
	_wcex.lpfnWndProc = WndProc;
	_wcex.hInstance = _hInstance;
	_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	_wcex.lpszClassName = L"WindowClass1";

	// register the window class
	if (!RegisterClassExW(&_wcex))
	{
		MessageBox(0, "Unable to register window class", 0, 0);
	}

	RECT windowRect = { 0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;
	
	_hWnd = CreateWindowEx(NULL,
						   "WindowClass1",    // name of the window class
						   "Graphics 2",   // title of the window
						   WS_OVERLAPPEDWINDOW,    // window style
						   300,    // x-position of the window
						   300,    // y-position of the window
						   _width,    // width of the window
						   _height,    // height of the window
						   NULL,    // we have no parent window, NULL
						   NULL,    // we aren't using menus, NULL
						   _hInstance,    // application handle
						   NULL);    // used with multiple windows, NULL

	if (!_hWnd)
	{
		MessageBox(0, "Unable to create window", 0, 0);
	}
	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);
	isAlive = true;
}

void DXWindow::Run()
{
	while (this->isAlive)
	{
		while (GetMessage(&_msg, NULL, 0, 0))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}
}

LRESULT DXWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		case WM_SIZE:
			_width = LOWORD(lParam);
			_height = HIWORD(lParam);
			//OnResize(wParam);
			//Render();
			break;
		
		// this message is read when the window is closed
		case WM_DESTROY:
			// close the application entirely
			PostQuitMessage(0);
			break;
	
		default:
			// Handle any messages the switch statement didn't
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// this is the main message handler for the program
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_thisWindow != NULL)
	{
		return _thisWindow->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
}

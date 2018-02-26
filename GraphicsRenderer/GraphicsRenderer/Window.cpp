#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow)
{
	//Set handlers and command line pointers
	_hInstance = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;

	//Set base width and height of window
	_width = DEFAULT_WIDTH;
	_height = DEFAULT_HEIGHT;

	this->SetUpWindow();
}

Window::~Window()
{
}

void Window::SetUpWindow()
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

	//Set the size of the rectangle used for the window
	RECT windowRect = { 0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;

	//Create window
	_hWnd = CreateWindowExW(NULL,
		L"WindowClass1",    // name of the window class
		L"Graphics 2",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		_width,    // width of the window
		_height,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		_hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  //Throw is failed
	if (!_hWnd)
	{
		MessageBox(0, "Unable to create window", 0, 0);
	}

	SetWindowLongPtrW(_hWnd, GWLP_USERDATA, (long)this);

	//Display window
	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);
}

LRESULT Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		//This message is read when the window is being resized
	case WM_SIZE:
		_width = LOWORD(lParam);
		_height = HIWORD(lParam);
		OnResize(wParam);
		this->Render();
		break;

		// This message is read when the window is being moved arround the screen
	case WM_MOVING:
		this->Render();
		break;

		// this message is read when the window is closed
	case WM_DESTROY:
		// close the application entirely
		Shutdown();
		PostQuitMessage(0);
		break;

	default:
		// Handle any messages the switch statement didn't
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

// this is the main message handler for the program
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* winPtr = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	if (winPtr != NULL)
	{
		return winPtr->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
}

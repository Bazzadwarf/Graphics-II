#pragma once
#include "Core.h"
#define DEFAULT_FRAMERATE 60
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define MAX_LOADSTRING 100

class DXWindow
{
public:
	DXWindow();
	DXWindow(HINSTANCE & hInstance,
			 HINSTANCE & hPrevInstance,
			 LPSTR & lpCmdLine,
			 int & nCmdShow);
	~DXWindow();

	void SetUpWindow();
	void Run();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND _hWnd;
	WNDCLASSEXW _wcex;
	MSG _msg;

	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR _lpCmdLine;
	int _nCmdShow;

	unsigned int _width;
	unsigned int _height;
	bool isAlive = false;
};


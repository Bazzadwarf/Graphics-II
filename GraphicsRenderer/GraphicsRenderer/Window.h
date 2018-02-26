#pragma once
#include "WindowsCore.h"
#include "Resource.h"

#define DEFAULT_FRAMERATE 60
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define MAX_LOADSTRING 100

class Window
{
public:
	Window(HINSTANCE & hInstance,
		HINSTANCE & hPrevInstance,
		LPSTR & lpCmdLine,
		int & nCmdShow);
	~Window();

	void SetUpWindow();

	virtual void OnResize(WPARAM wParam) {};
	virtual void Run() {};
	virtual void Render() {};
	virtual void Update() {};
	virtual void Shutdown() {};

	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HWND		 _hWnd;
	WNDCLASSEXW  _wcex;
	MSG			 _msg;

	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR	  _lpCmdLine;
	int		  _nCmdShow;

	unsigned int _width;
	unsigned int _height;
	bool		 _isAlive = true;
};


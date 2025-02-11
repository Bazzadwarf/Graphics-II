#include "Framework.h"
#include "Graphics.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Graphics dxWnd = Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	dxWnd.InitialiseDirectX();
	dxWnd.Run();
	return 1;
}

Framework::Framework(HINSTANCE & hInstance,
	HINSTANCE & hPrevInstance,
	LPSTR & lpCmdLine,
	int & nCmdShow)
{
	_hInstance = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;
}


Framework::~Framework()
{
}

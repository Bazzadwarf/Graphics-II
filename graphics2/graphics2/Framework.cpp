#include "Framework.h"

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{	
	Framework framework(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	framework.Setup();
	framework.Run();

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

void Framework::Setup()
{
	if (_window == nullptr)
	{
		this->_window = new DXWindow(_hInstance, _hPrevInstance, _lpCmdLine, _nCmdShow);
		_window->SetUpWindow();
		_window->SetUpDirectX();
		_window->Run();
	}
}

void Framework::Run()
{

}
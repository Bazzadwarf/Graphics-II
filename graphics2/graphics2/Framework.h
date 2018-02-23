#pragma once
#include "DXWindow.h"

class Framework
{
public:
	Framework(HINSTANCE & hInstance,
			  HINSTANCE & hPrevInstance,
			  LPSTR & lpCmdLine,
			  int & nCmdShow);
	~Framework();

	void Setup();
	void Run();

private:
	DXWindow * _window = nullptr;

	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR _lpCmdLine;
	int _nCmdShow;
};


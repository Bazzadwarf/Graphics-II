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

private:
	HINSTANCE _hInstance;
	HINSTANCE _hPrevInstance;
	LPSTR	  _lpCmdLine;
	int		  _nCmdShow;
};


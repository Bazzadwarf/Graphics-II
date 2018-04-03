#pragma once
#include "DXWindow.h"
#include "CubeNode.h"

class Graphics : public DXWindow
{
public:

	Graphics(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow) : DXWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
	~Graphics() {};

	void CreateSceneGraph();
	void UpdateSceneGraph();

private:
	float _rotationAngle;
};


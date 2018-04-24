#pragma once
#include "DXWindow.h"
#include "CubeNode.h"
#include "MeshNode.h"
#include "TerrainNode.h"

#define VK_A 0x41
#define VK_D 0x44
#define VK_S 0x53
#define VK_W 0x57

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


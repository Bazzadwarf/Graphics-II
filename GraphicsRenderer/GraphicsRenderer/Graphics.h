#pragma once
#include "DXWindow.h"
#include "CubeNode.h"

class Graphics : public DXWindow
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();
};


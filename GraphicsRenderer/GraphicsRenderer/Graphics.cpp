#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	CubeNode cube(L"cube", XMFLOAT4(2, 2, 2, 2), L"Woodbox.bmp");
}

void Graphics::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
}

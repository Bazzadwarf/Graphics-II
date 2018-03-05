#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	CubeNode cube(L"aaaa" , XMFLOAT4(2, 2, 2, 2), L"Woodbox.bmp");
}

void Graphics::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
}

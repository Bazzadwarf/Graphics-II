#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	SceneNodePointer cube = make_shared<CubeNode>(L"cube", XMFLOAT3(4,4,4), wchar_t(L"woodbox.bmp"));
	sceneGraph->Add(cube);
	sceneGraph->Initialise();
}

void Graphics::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
}

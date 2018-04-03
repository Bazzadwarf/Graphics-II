#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	SceneNodePointer cube = make_shared<CubeNode>(L"cube", XMFLOAT3(2,2,2), L"woodbox.bmp");
	sceneGraph->Add(cube);
	//SceneNodePointer cube2 = make_shared<CubeNode>(L"cube2", XMFLOAT3(4, 4, 4), L"3.png");
	//sceneGraph->Add(cube2);
	sceneGraph->Initialise();
}

void Graphics::UpdateSceneGraph()
{
	_rotationAngle += 1.0f;
	SceneGraphPointer sceneGraph = GetSceneGraph();
	sceneGraph->Find(L"cube")->SetLocalTransform(XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), _rotationAngle * 0.5f * XM_PI / 180.0f));
}

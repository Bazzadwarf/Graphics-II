#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	//SceneNodePointer cube = make_shared<CubeNode>(L"cube", XMFLOAT3(2,2,2), L"woodbox.bmp");
	//sceneGraph->Add(cube);

	shared_ptr<MeshNode> node = make_shared<MeshNode>(L"Plane1", L"airplane.x");
	node->Update(XMMatrixTranslation(0, 0, -10.0f));
	sceneGraph->Add(node);

	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"rollinghills.raw");
	terrain->Update(XMMatrixScaling(1.0f, 1.0f, 1.0f));
	sceneGraph->Add(terrain);

	sceneGraph->Initialise();
}

void Graphics::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	_rotationAngle -= 1.0f;
	//sceneGraph->Find(L"cube")->SetLocalTransform(XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), _rotationAngle * 0.5f * XM_PI / 180.0f));
	sceneGraph->Find(L"Plane1")->Update(XMMatrixTranslation(0, 0, _rotationAngle));
}

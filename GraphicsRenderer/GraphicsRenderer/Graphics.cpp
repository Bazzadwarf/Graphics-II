#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	
	GetCamera()->SetCameraPosition(0, 1000, 0);
	
	//SceneNodePointer cube = make_shared<CubeNode>(L"cube", XMFLOAT3(2,2,2), L"woodbox.bmp");
	//sceneGraph->Add(cube);

	shared_ptr<MeshNode> node = make_shared<MeshNode>(L"Plane1", L"airplane.x");
	node->Update(XMMatrixTranslation(0, 0, -10.0f));
	sceneGraph->Add(node);

	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"rollinghills.raw");
	terrain->Update(XMMatrixScaling(2.0f, 1.0f, 2.0f));
	sceneGraph->Add(terrain);

	sceneGraph->Initialise();
}

void Graphics::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	_rotationAngle -= 1.0f;
	//sceneGraph->Find(L"cube")->SetLocalTransform(XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), _rotationAngle * 0.5f * XM_PI / 180.0f));
	sceneGraph->Find(L"Plane1")->Update(XMMatrixTranslation(0, 0, _rotationAngle));

	if (GetAsyncKeyState(VK_UP) < 0)
	{
		GetCamera()->SetForwardBack(1);
	}

	if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		GetCamera()->SetForwardBack(-1);
	}

	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		GetCamera()->SetYaw(-1);
	}

	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		GetCamera()->SetYaw(1);
	}

	if (GetAsyncKeyState(VK_PRIOR) < 0)
	{
		GetCamera()->SetPitch(-1);
	}

	if (GetAsyncKeyState(VK_NEXT) < 0)
	{
		GetCamera()->SetPitch(1);
	}
}

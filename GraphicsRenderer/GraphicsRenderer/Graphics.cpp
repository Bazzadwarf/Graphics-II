#include "Graphics.h"

void Graphics::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	
	GetCamera()->SetCameraPosition(0, 50, 100);
	
	//SceneNodePointer cube = make_shared<CubeNode>(L"cube", XMFLOAT3(2,2,2), L"woodbox.bmp");
	//sceneGraph->Add(cube);

	shared_ptr<MeshNode> node = make_shared<MeshNode>(L"Plane1", L"airplane.x");
	//node->Update(XMMatrixTranslation(0, 0, -10.0f));
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

	if (GetAsyncKeyState(VK_W) < 0 && GetAsyncKeyState(VK_SHIFT) < 0)
	{
		this->GetCamera()->SetForwardBack(10);
	}
	else if (GetAsyncKeyState(VK_W) < 0)
	{
		this->GetCamera()->SetForwardBack(1);
	}

	if (GetAsyncKeyState(VK_S) < 0)
	{
		this->GetCamera()->SetForwardBack(-1);
	}

	if (GetAsyncKeyState(VK_D) < 0)
	{
		this->GetCamera()->SetLeftRight(1);
	}

	if (GetAsyncKeyState(VK_A) < 0)
	{
		this->GetCamera()->SetLeftRight(-1);
	}

	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		this->GetCamera()->SetYaw(-1);
	}

	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		this->GetCamera()->SetYaw(1);
	}

	if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		this->GetCamera()->SetPitch(-1);
	}

	if (GetAsyncKeyState(VK_UP) < 0)
	{
		this->GetCamera()->SetPitch(1);
	}

	if (GetAsyncKeyState(VK_PRIOR) < 0)
	{
		//this->GetCamera()->
	}

	if (GetAsyncKeyState(VK_ESCAPE) < 0)
	{
		Shutdown();
		PostQuitMessage(0);
	}
}

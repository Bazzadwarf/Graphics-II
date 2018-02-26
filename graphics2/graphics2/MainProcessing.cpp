#include "MainProcessing.h"

void MainProcessing::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	ComPtr<ID3D11Device> device = GetDXFramework()->GetDevice();
}

void MainProcessing::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
}

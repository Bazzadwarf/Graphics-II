#include "SceneGraph.h"

bool SceneGraph::Initialise()
{
	return false;
}

void SceneGraph::Update(FXMMATRIX & currentWindow)
{

}

void SceneGraph::Render()
{

}

void SceneGraph::Shutown()
{

}

void SceneGraph::Add(SceneNodePointer node)
{
	_sceneNodeCollection.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
	size_t sceneNodeSize = _sceneNodeCollection.size();
	for (size_t i = 0; i < sceneNodeSize; i++)
	{
		if (_sceneNodeCollection[i] == node)
		{
			_sceneNodeCollection.erase(_sceneNodeCollection.begin() + i);
			return;
		}
	}
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	return SceneNodePointer();
}

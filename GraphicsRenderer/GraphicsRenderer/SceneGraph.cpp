#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
	for (size_t i = 0; i < _children.size(); i++)
	{
		if (!(_children.at(i)->Initialise()))
		{
			return false;
		}
	}
	return true;
}

void SceneGraph::Update(FXMMATRIX & currentWorldTransformation)
{
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->Update(currentWorldTransformation);
	}
}

void SceneGraph::Render(void)
{
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->Render();
	}
}

void SceneGraph::Shutdown(void)
{
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node)
{
	_children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	return SceneNodePointer();
}

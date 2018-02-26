#pragma once
#include "SceneNode.h"
#include <vector>


class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph() {};

	virtual bool Initialise();
	virtual void Update(FXMMATRIX& currentWindow);
	virtual void Render();
	virtual void Shutdown();

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

private:
	vector<SceneNodePointer> _sceneNodeCollection;
};

typedef  shared_ptr<SceneGraph> SceneGraphPointer;
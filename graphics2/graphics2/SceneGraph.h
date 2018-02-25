#pragma once
#include "SceneNode.h"
#include <vector>

class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph() {};

	bool Initialise();
	void Update(FXMMATRIX& currentWindow);
	void Render();
	void Shutown();

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

private:
	vector<SceneNodePointer> _sceneNodeCollection;
};

typedef  shared_ptr<SceneGraph> SceneGraphPointer;
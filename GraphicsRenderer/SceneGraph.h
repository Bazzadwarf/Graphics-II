#pragma once
#include "SceneNode.h"
#include <vector>

class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph(void) {};

	bool Initialise(void);
	void Update(FXMMATRIX& currentWorldTransformation);
	void Render(void);
	void Shutdown(void);

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

private:
	// Here you need to add whatever collection you are going to
	// use to store the children of this scene graph

	vector<SceneNodePointer> _children;
};

typedef shared_ptr<SceneGraph>			 SceneGraphPointer;

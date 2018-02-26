#pragma once
#include "SceneNode.h"

class CubeNode : public SceneNode
{
public:
	CubeNode(XMFLOAT4 c) : SceneNode(L"Cube") { _colour = c; };
	~CubeNode() {};

	virtual bool Initialise();
	virtual void Update();
	virtual void Render();
	virtual void Shutdown();


private:
	XMFLOAT4 _colour;
};


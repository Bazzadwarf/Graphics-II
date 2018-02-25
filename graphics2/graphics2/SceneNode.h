#pragma once
#include "Core.h"

class SceneNode;

typedef shared_ptr<SceneNode> SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) { _name = name; XMStoreFloat4x4(&_worldTransformation, XMMatrixIdentity()); };
	~SceneNode() {};

	void SetWorldTransform(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_worldTransformation, worldTransformation); };

protected:
	XMFLOAT4X4	_worldTransformation;
	XMFLOAT4X4 _combinedWorldTransformation;
	wstring _name;
};

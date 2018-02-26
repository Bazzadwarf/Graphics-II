#pragma once
#include "core.h"

class SceneNode;

typedef shared_ptr<SceneNode> SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) { _name = name; XMStoreFloat4x4(&_worldTransformation, XMMatrixIdentity()); };
	~SceneNode() {};

	virtual bool Initialise() = 0;
	virtual void Update(FXMMATRIX& currentWorldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, XMLoadFloat4x4(&_worldTransformation) * currentWorldTransformation); };
	virtual void Render() = 0	;
	virtual void Shutdown() = 0;

	void SetWorldTransform(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_worldTransformation, worldTransformation); };

	virtual void Add(SceneNodePointer node) {};
	virtual void Remove(SceneNodePointer node) {};
	virtual SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; };

protected:
	XMFLOAT4X4	_worldTransformation;
	XMFLOAT4X4 _combinedWorldTransformation;
	wstring _name;
};

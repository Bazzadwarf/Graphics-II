#pragma once
#include "WindowsCore.h"
#include "DirectXCore.h"

class SceneNode;

typedef shared_ptr<SceneNode> SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) { _name = name; XMStoreFloat4x4(&_localTransformation, XMMatrixIdentity()); };
	~SceneNode(void) {};

	// Core methods
	virtual bool Initialise() = 0;
	virtual void Update(FXMMATRIX& currentWorldTransformation) { XMStoreFloat4x4(&_worldTransformation, XMLoadFloat4x4(&_localTransformation) * currentWorldTransformation); }
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	void SetLocalTransform(FXMMATRIX& localTransformation) { XMStoreFloat4x4(&_localTransformation, localTransformation); }

	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

protected:
	XMFLOAT4X4			_localTransformation;			//local transformation
	XMFLOAT4X4			_worldTransformation;			//global transformation
	wstring				_name;
};
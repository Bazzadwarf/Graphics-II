#pragma once
#include "SceneNode.h"
#include "DXWindow.h"
#include "WICTextureLoader.h"

class CubeNode : public SceneNode
{
	struct Vertex
	{
		XMFLOAT4 Position;
		XMFLOAT2 TextureCoordinate;
	};

	struct CBUFFER
	{
		XMMATRIX CompleteTransformation;
		XMFLOAT4 AmbientColour;
	};

public:
	CubeNode(wstring name, XMFLOAT3 size, wchar_t* file) : SceneNode(name) { _size = size; _file = file; };
	~CubeNode() {};
	
	virtual bool Initialise();
	virtual void Update(FXMMATRIX& currentWorldTransformation);
	virtual void Render();
	virtual void Shutdown();

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildTexture();

private:
	XMFLOAT3 _size;
	wchar_t* _file;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11PixelShader>		_pixelShader;

	ComPtr<ID3D11InputLayout>		_layout;

	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3D11ShaderResourceView> _texture;
};


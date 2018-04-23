#pragma once
#include "SceneNode.h"
#include "DXWindow.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include <vector>
#include <fstream>

#define NUMBER_OF_ROWS			257
#define NUMBER_OF_COLUMNS	    257
#define MAGNITUDE				100

class TerrainNode : public SceneNode
{
	struct VERTEX
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 TexCoord;
		XMFLOAT2 BlendMapTexCoord;
	};
	
	struct CBUFFER
	{
		XMMATRIX completeTransformation;
		XMMATRIX worldTransformation;
		XMFLOAT4 cameraPosition;
		XMVECTOR lightVector;		// the light's vector
		XMFLOAT4 lightColor;		// the light's color
		XMFLOAT4 ambientColor;	// the ambient light's color
		XMFLOAT4 diffuseColor;	// The diffuse color (reflection cooefficient)
		XMFLOAT4 specularColor;	// The specular color (reflection cooefficient)
		float  shininess;		// The shininess factor
		XMFLOAT3 padding;
	};

public:
	TerrainNode(wstring name, wstring file) : SceneNode(name) { _file = file; };
	~TerrainNode() {};

	bool Initialise();
	void Render();
	void Shutdown();

	void GenerateGrid();

	void GenerateNormals();

	void BuildGeometryBuffers();
	void BuildConstantBuffer();
	void BuildShaders();
	void BuildVertexLayout();

	void BuildRendererStates();

	bool LoadHeightMap(wstring heightMapFilename);

	void LoadTerrainTextures();
	void GenerateBlendMap();

private:	
	DXWindow * _dxframework;
	
	wstring _file;

	vector<VERTEX> _vertices;
	vector<UINT> _indices;
	vector<float> _heightValues;

	int _gridSize = 256;

	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;

	ComPtr<ID3D11InputLayout>		_layout;

	ComPtr<ID3D11RasterizerState> _defaultRasteriserState;
	ComPtr<ID3D11RasterizerState> _wireframeRasteriserState;

	ComPtr<ID3D11ShaderResourceView> _texturesResourceView;
	ComPtr<ID3D11ShaderResourceView> _blendMapResourceView;
};


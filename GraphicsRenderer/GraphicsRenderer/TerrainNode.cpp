#include "TerrainNode.h"

bool TerrainNode::Initialise()
{
	_dxframework = DXWindow::GetDXFramework();
	
	//1. Load the height map

	//2. Generate the vertices and indices for the polygongs in the terrain grid
	
	VERTEX tempVertex;
	tempVertex.Normal = XMFLOAT3(0, 0, 0);
	tempVertex.TexCoord = XMFLOAT2(0, 0);
	tempVertex.BlendMapTexCoord = XMFLOAT2(0, 0);

	for (float z = 0; z < _gridSize; z++)
	{
		for (float x = 0; x < _gridSize; x++)
		{
			int square = x + (z * _gridSize);
			float tlx = (square % _gridSize) - (_gridSize / 2);
			float tlz = (floor(square / _gridSize) - (_gridSize / 2));

			////////////////////////////////////////////////////////////////////////////////////////////////////

			//top left vertex
			tempVertex.Position = XMFLOAT3(tlx, 0, tlz);
			_vertices.push_back(tempVertex);

			//top right vertex
			tempVertex.Position = XMFLOAT3(tlx + 1, 0, tlz);
			_vertices.push_back(tempVertex);

			//bottom left vertex
			tempVertex.Position = XMFLOAT3(tlx, 0, tlz + 1);
			_vertices.push_back(tempVertex);

			//bottom right vertex
			tempVertex.Position = XMFLOAT3(tlx + 1, 0, tlz + 1);
			_vertices.push_back(tempVertex);

			////////////////////////////////////////////////////////////////////////////////////////////////////

			//Triangle 1 v1
			_indices.push_back(square * 4);				//Get the 1st vertex in the square for the 1st triangle

			//Triangle 1 v2
			_indices.push_back((square * 4) + 1);		//Get the 2nd vertex in the square for the 1st triangle
			
			//Triangle 1 v3
			_indices.push_back((square * 4) + 2);		//Get the 3rd vertex in the square for the 1st triangle
														
			//Triangle 2 v3
			_indices.push_back((square * 4) + 2);		//Get the 3rd vertex in the square for the 2nd triangle

			//Triangle 2 v2
			_indices.push_back((square * 4) + 1);		//Get the 2nd vertex in the square for the 2nd triangle

			//Triangle 2 v4
			_indices.push_back((square * 4) + 3);		//Get the 4th vertex in the square for the 2nd triangle

		}
	}

	//3. Generate the normals for the polygons in the terrain grid

	//4. Create the vertex and index buffers for the terrain polygons
	this->BuildRendererStates();
	
	this->BuildShaders();
	this->BuildVertexLayout();
	this->BuildConstantBuffer();
	this->BuildGeometryBuffers();
	
	
	

	//5. Load the tile textures

	//6. Generate a blend map for the terrain

	return true;
}

void TerrainNode::Render()
{
	XMFLOAT4X4 projectionTransformation = DXWindow::GetDXFramework()->GetProjectionTransformation();
	XMFLOAT4X4 viewTransformation = DXWindow::GetDXFramework()->GetViewTransformation();

	XMMATRIX completeTransformation = XMLoadFloat4x4(&_worldTransformation) * XMLoadFloat4x4(&viewTransformation) * XMLoadFloat4x4(&projectionTransformation);
	
	CBUFFER cBuffer;
	cBuffer.completeTransformation = completeTransformation;
	cBuffer.worldTransformation = XMLoadFloat4x4(&_localTransformation);
	cBuffer.cameraPosition = XMFLOAT4(0.0f, 50.0f, -500.0f, 0.0f);
	cBuffer.lightVector = XMVector4Normalize(XMVectorSet(0.0f, 01.0f, 1.0f, 0.0f));
	cBuffer.lightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.ambientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.shininess = 1;

	_dxframework->GetDeviceContext()->VSSetShader(_vertexShader.Get(), 0, 0);
	_dxframework->GetDeviceContext()->PSSetShader(_pixelShader.Get(), 0, 0);
	_dxframework->GetDeviceContext()->IASetInputLayout(_layout.Get());

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	_dxframework->GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_dxframework->GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_dxframework->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_dxframework->GetDeviceContext()->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	_dxframework->GetDeviceContext()->RSSetState(_wireframeRasteriserState.Get());	_dxframework->GetDeviceContext()->DrawIndexed(static_cast<UINT>(_indices.size()), 0, 0);	_dxframework->GetDeviceContext()->RSSetState(_defaultRasteriserState.Get());
}

void TerrainNode::Shutdown()
{
}

void TerrainNode::BuildGeometryBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(VERTEX) * _vertices.size();
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = &_vertices[0];

	ThrowIfFailed(_dxframework->GetDevice()->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * _indices.size();
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &_indices[0];

	ThrowIfFailed(_dxframework->GetDevice()->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));

}

void TerrainNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ThrowIfFailed(_dxframework->GetDevice()->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TerrainNode::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(L"TerrainShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VShader", "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(_dxframework->GetDevice()->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

	// Compile pixel shader
	hr = D3DCompileFromFile(L"TerrainShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PShader", "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(_dxframework->GetDevice()->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}

void TerrainNode::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
	{ "Position",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "Normal",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TexCoord", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BlendMapTexCoord", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ThrowIfFailed(_dxframework->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TerrainNode::BuildRendererStates()
{
	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	ThrowIfFailed(_dxframework->GetDevice()->CreateRasterizerState(&rasteriserDesc, _defaultRasteriserState.GetAddressOf()));
	rasteriserDesc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowIfFailed(_dxframework->GetDevice()->CreateRasterizerState(&rasteriserDesc, _wireframeRasteriserState.GetAddressOf()));
}


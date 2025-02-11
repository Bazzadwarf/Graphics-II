#include "TerrainNode.h"

bool TerrainNode::Initialise()
{
	_dxframework = DXWindow::GetDXFramework();
	
	//1. Load the height map
	this->LoadHeightMap(_file);

	//2. Generate the vertices and indices for the polygongs in the terrain grid
	this->GenerateGrid();

	//3. Generate the normals for the polygons in the terrain grid
	this->GenerateNormals();

	//4. Create the vertex and index buffers for the terrain polygons
	this->BuildRendererStates();
	this->BuildGeometryBuffers();
	this->BuildShaders();
	this->BuildVertexLayout();
	this->BuildConstantBuffer();

	//5. Load the tile textures
	this->LoadTerrainTextures();

	//6. Generate a blend map for the terrain
	this->GenerateBlendMap();

	return true;
}

void TerrainNode::Render()
{
	XMFLOAT4X4 projectionTransformation = DXWindow::GetDXFramework()->GetProjectionTransformation();
	XMMATRIX viewTransformation = _dxframework->GetCamera()->GetViewMatrix();

	XMMATRIX completeTransformation = XMLoadFloat4x4(&_worldTransformation) * viewTransformation * XMLoadFloat4x4(&projectionTransformation);
	
	CBUFFER cBuffer;
	cBuffer.completeTransformation = completeTransformation;
	cBuffer.worldTransformation = XMLoadFloat4x4(&_worldTransformation);
	XMStoreFloat4(&cBuffer.cameraPosition , _dxframework->GetCamera()->GetCameraPosition());
	cBuffer.lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f));
	cBuffer.lightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.ambientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.diffuseColor = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cBuffer.specularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cBuffer.shininess = 0;

	_dxframework->GetDeviceContext()->VSSetShader(_vertexShader.Get(), 0, 0);
	_dxframework->GetDeviceContext()->PSSetShader(_pixelShader.Get(), 0, 0);
	_dxframework->GetDeviceContext()->IASetInputLayout(_layout.Get());

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	_dxframework->GetDeviceContext()->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_dxframework->GetDeviceContext()->UpdateSubresource(_constantBuffer.Get(), 0, 0, &cBuffer, 0, 0);

	_dxframework->GetDeviceContext()->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_dxframework->GetDeviceContext()->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_dxframework->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_dxframework->GetDeviceContext()->PSSetShaderResources(0, 1, _blendMapResourceView.GetAddressOf());
	_dxframework->GetDeviceContext()->PSSetShaderResources(1, 1, _texturesResourceView.GetAddressOf());

	/*_dxframework->GetDeviceContext()->RSSetState(_wireframeRasteriserState.Get());*/
	_dxframework->GetDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
	/*_dxframework->GetDeviceContext()->RSSetState(_defaultRasteriserState.Get());*/
	
}

void TerrainNode::Shutdown()
{
}

void TerrainNode::GenerateGrid()
{
	VERTEX tempVertex;
	tempVertex.Normal = XMFLOAT3(0, 0, 0);
	tempVertex.TexCoord = XMFLOAT2(0, 0);
	tempVertex.BlendMapTexCoord = XMFLOAT2(0, 0);

	for (int z = 0; z < _gridSize; z++)
	{
		for (int x = 0; x < _gridSize; x++)
		{
			int square = x + (z * _gridSize);
			float tlx = float((square % _gridSize) - (_gridSize / 2));
			float tlz = float((floor(square / (-_gridSize)) + (_gridSize / 2)));

			//top left vertex
			tempVertex.Position = XMFLOAT3(tlx, _heightValues[((z * 257) + x)] * MAGNITUDE, tlz);
			tempVertex.TexCoord = XMFLOAT2(0, 0);
			tempVertex.BlendMapTexCoord = XMFLOAT2(x / 257.0f, z / 257.0f);
			_vertices.push_back(tempVertex);

			//top right vertex
			tempVertex.Position = XMFLOAT3(tlx + 1, _heightValues[((z * 257) + x) + 1] * MAGNITUDE, tlz);
			tempVertex.TexCoord = XMFLOAT2(1, 0);
			tempVertex.BlendMapTexCoord = XMFLOAT2((x + 1) / 257.0f, z / 257.0f);
			_vertices.push_back(tempVertex);

			//bottom left vertex
			tempVertex.Position = XMFLOAT3(tlx, _heightValues[((z * 257) + x) + 257] * MAGNITUDE, tlz - 1);
			tempVertex.TexCoord = XMFLOAT2(0, 1);
			tempVertex.BlendMapTexCoord = XMFLOAT2(x / 257.0f, (z + 1) / 257.0f);
			_vertices.push_back(tempVertex);

			//bottom right vertex
			tempVertex.Position = XMFLOAT3(tlx + 1, _heightValues[((z * 257) + x) + 258] * MAGNITUDE, tlz - 1);
			tempVertex.TexCoord = XMFLOAT2(1, 1);
			tempVertex.BlendMapTexCoord = XMFLOAT2((x + 1) / 257.0f, (z + 1) / 257.0f);
			_vertices.push_back(tempVertex);

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
}

void TerrainNode::GenerateNormals()
{
	int squareCount = _gridSize * _gridSize;
	
	//for each square in the grid
	for (int i = 0; i < squareCount; i++)
	{
		
		//Calculate Normal for polygon 1
		XMVECTOR p1a = XMVectorSet(_vertices[(i * 4)].Position.x - _vertices[(i * 4) + 1].Position.x,
								   _vertices[(i * 4)].Position.y - _vertices[(i * 4) + 1].Position.y,
								   _vertices[(i * 4)].Position.z - _vertices[(i * 4) + 1].Position.z,
								   0);

		XMVECTOR p1b = XMVectorSet(_vertices[(i * 4)].Position.x - _vertices[(i * 4) + 2].Position.x,
								   _vertices[(i * 4)].Position.y - _vertices[(i * 4) + 2].Position.y,
								   _vertices[(i * 4)].Position.z - _vertices[(i * 4) + 2].Position.z,
								   0);

		XMVECTOR p1normal = XMVector3Cross(p1b, p1a);

		//Calculate Normal for polygon 2
		XMVECTOR p2a = XMVectorSet(_vertices[(i * 4) + 2].Position.x - _vertices[(i * 4) + 1].Position.x,
								   _vertices[(i * 4) + 2].Position.y - _vertices[(i * 4) + 1].Position.y,
								   _vertices[(i * 4) + 2].Position.z - _vertices[(i * 4) + 1].Position.z,
								   0);

		XMVECTOR p2b = XMVectorSet(_vertices[(i * 4) + 2].Position.x - _vertices[(i * 4) + 3].Position.x,
								   _vertices[(i * 4) + 2].Position.y - _vertices[(i * 4) + 3].Position.y,
								   _vertices[(i * 4) + 2].Position.z - _vertices[(i * 4) + 3].Position.z,
								   0);

		XMVECTOR p2normal = XMVector3Cross(p2b, p2a);

		//Set normal for the first vertex from polygon 1
		XMVECTOR tempVert = p1normal;
		tempVert = XMVector3Normalize(tempVert);
		XMStoreFloat3(&_vertices[(i * 4)].Normal, tempVert);

		//Set normal for the last vertex from polygon 2
		tempVert = p2normal;
		tempVert = XMVector3Normalize(tempVert);
		XMStoreFloat3(&_vertices[(i * 4) + 3].Normal, tempVert);

		//Set normal for the second vertex from the average of polygon 1 & 2
		tempVert = (p1normal + p2normal) / 2;
		tempVert = XMVector3Normalize(tempVert);
		XMStoreFloat3(&_vertices[(i * 4) + 1].Normal, tempVert);

		//Set normal for the third vertex from the average of polygon 1 & 2
		XMStoreFloat3(&_vertices[(i * 4) + 2].Normal, tempVert);

	}
}

void TerrainNode::BuildGeometryBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(VERTEX) * (UINT)_vertices.size();
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = &_vertices[0];

	ThrowIfFailed(_dxframework->GetDevice()->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * (UINT)_indices.size();
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

bool TerrainNode::LoadHeightMap(wstring heightMapFilename)
{
	int fileSize = (NUMBER_OF_COLUMNS + 1) * (NUMBER_OF_ROWS + 1);
	BYTE * rawFileValues = new BYTE[fileSize];

	std::ifstream inputHeightMap;
	inputHeightMap.open(heightMapFilename.c_str(), std::ios_base::binary);
	if (!inputHeightMap)
	{
		return false;
	}

	inputHeightMap.read((char*)rawFileValues, fileSize);
	inputHeightMap.close();

	// Normalise BYTE values to the range 0.0f - 1.0f;
	for (int i = 0; i < fileSize; i++)
	{
		_heightValues.push_back((float)rawFileValues[i] / 255);
	}
	delete[] rawFileValues;
	return true;
}

void TerrainNode::LoadTerrainTextures()
{
	// Change the paths below as appropriate for your use
	wstring terrainTextureNames[5] = { L"grass.dds", L"darkdirt.dds", L"stone.dds", L"lightdirt.dds", L"snow.dds" };

	// Load the textures from the files
	ComPtr<ID3D11Resource> terrainTextures[5];
	for (int i = 0; i < 5; i++)
	{
		ThrowIfFailed(CreateDDSTextureFromFileEx(_dxframework->GetDevice().Get(),
			_dxframework->GetDeviceContext().Get(),
			terrainTextureNames[i].c_str(),
			0,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			terrainTextures[i].GetAddressOf(),
			nullptr
		));
	}
	// Now create the Texture2D arrary.  We assume all textures in the
	// array have the same format and dimensions

	D3D11_TEXTURE2D_DESC textureDescription;
	ComPtr<ID3D11Texture2D> textureInterface;
	terrainTextures[0].As<ID3D11Texture2D>(&textureInterface);
	textureInterface->GetDesc(&textureDescription);

	D3D11_TEXTURE2D_DESC textureArrayDescription;
	textureArrayDescription.Width = textureDescription.Width;
	textureArrayDescription.Height = textureDescription.Height;
	textureArrayDescription.MipLevels = textureDescription.MipLevels;
	textureArrayDescription.ArraySize = 5;
	textureArrayDescription.Format = textureDescription.Format;
	textureArrayDescription.SampleDesc.Count = 1;
	textureArrayDescription.SampleDesc.Quality = 0;
	textureArrayDescription.Usage = D3D11_USAGE_DEFAULT;
	textureArrayDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureArrayDescription.CPUAccessFlags = 0;
	textureArrayDescription.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureArray = 0;
	ThrowIfFailed(_dxframework->GetDevice()->CreateTexture2D(&textureArrayDescription, 0, textureArray.GetAddressOf()));

	// Copy individual texture elements into texture array.

	for (UINT i = 0; i < 5; i++)
	{
		// For each mipmap level...
		for (UINT mipLevel = 0; mipLevel < textureDescription.MipLevels; mipLevel++)
		{
			_dxframework->GetDeviceContext()->CopySubresourceRegion(textureArray.Get(),
				D3D11CalcSubresource(mipLevel, i, textureDescription.MipLevels),
				NULL,
				NULL,
				NULL,
				terrainTextures[i].Get(),
				mipLevel,
				nullptr
			);
		}
	}

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
	viewDescription.Format = textureArrayDescription.Format;
	viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDescription.Texture2DArray.MostDetailedMip = 0;
	viewDescription.Texture2DArray.MipLevels = textureArrayDescription.MipLevels;
	viewDescription.Texture2DArray.FirstArraySlice = 0;
	viewDescription.Texture2DArray.ArraySize = 5;

	ThrowIfFailed(_dxframework->GetDevice()->CreateShaderResourceView(textureArray.Get(), &viewDescription, _texturesResourceView.GetAddressOf()));
}

void TerrainNode::GenerateBlendMap()
{
	DWORD blendMap[NUMBER_OF_ROWS * NUMBER_OF_COLUMNS];

	for (DWORD z = 0; z < NUMBER_OF_COLUMNS; z++)
	{
		for (DWORD x = 0; x < NUMBER_OF_ROWS; x++)
		{
			// Calculate the appropriate blend colour for the 
			// current location in the blend map.  This has been
			// left as an exercise for you.  You need to calculate
			// appropriate values for the r, g, b and a values (each
			// between 0 and 255) and then combine then into a DWORD
			// (32-bit value) to store in the blend map.

			// The values of (0, 0, 0, 0) will only show the grass texture.

			// The R value determins how much dirt will be blended in.
			byte r = 0;
			if (_heightValues[(z * NUMBER_OF_COLUMNS) + x] >= 0.1f)
			{
				r = 64;
			}

			// The G value determins how much stone will be blended in.
			byte g = 0;
			if (_heightValues[(z * NUMBER_OF_COLUMNS) + x] >= 0.4f && _heightValues[(z * NUMBER_OF_COLUMNS) + x] <= 0.6f)
			{
				g = 64;
			}
			else if (_heightValues[(z * NUMBER_OF_COLUMNS) + x] >= 0.6f && _heightValues[(z * NUMBER_OF_COLUMNS) + x] <= 0.8f)
			{
				g = 200;
			}
			
			// The B value determins how much light dirt will be blended in.
			byte b = 0;
			if (_heightValues[(z * NUMBER_OF_COLUMNS) + x] >= 0.2f && _heightValues[(z * NUMBER_OF_COLUMNS) + x] <= 0.6f)
			{
				b = 64;
			}
			
			// The A value determins how much snow will be blended in.
			byte a = 0;
			if (_heightValues[(z * NUMBER_OF_COLUMNS) + x] >= 0.6f)
			{
				g = 255;
			}
			
			blendMap[(z * NUMBER_OF_COLUMNS) + x] = (r | (g << 8) | (b << 16) | (a << 24));
		}
	}
	// Now create the texture from the raw blend map data
	D3D11_TEXTURE2D_DESC blendMapDescription;
	blendMapDescription.Width = NUMBER_OF_ROWS;
	blendMapDescription.Height = NUMBER_OF_COLUMNS;
	blendMapDescription.MipLevels = 1;
	blendMapDescription.ArraySize = 1;
	blendMapDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	blendMapDescription.SampleDesc.Count = 1;
	blendMapDescription.SampleDesc.Quality = 0;
	blendMapDescription.Usage = D3D11_USAGE_DEFAULT;
	blendMapDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	blendMapDescription.CPUAccessFlags = 0;
	blendMapDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA blendMapInitialisationData;
	blendMapInitialisationData.pSysMem = &blendMap;
	blendMapInitialisationData.SysMemPitch = 4 * NUMBER_OF_ROWS;

	ComPtr<ID3D11Texture2D> blendMapTexture;
	ThrowIfFailed(_dxframework->GetDevice()->CreateTexture2D(&blendMapDescription, &blendMapInitialisationData, blendMapTexture.GetAddressOf()));

	// Create a resource view for the blend map.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
	viewDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDescription.Texture2D.MostDetailedMip = 0;
	viewDescription.Texture2D.MipLevels = 1;

	ThrowIfFailed(_dxframework->GetDevice()->CreateShaderResourceView(blendMapTexture.Get(), &viewDescription, _blendMapResourceView.GetAddressOf()));
}

float TerrainNode::GetHeightAtPoint(float x, float z)
{
	// Calculate the cell that we are in
	_terrainStartX = _vertices[0].Position.x;
	_terrainStartZ = _vertices[0].Position.z;

	int cellX = (int)((x - _terrainStartX) / _spacing);
	int cellZ = (int)((_terrainStartZ - z) / _spacing);

	// Determine the value equal to the difference between the point z and the edge of the cell
	float dz = z - _vertices[(z * NUMBER_OF_COLUMNS) + x].Position.z;

	// Determine the value equal to the difference between the point x and the edge of the cell
	float dx = x - _vertices[(z * NUMBER_OF_COLUMNS) + x].Position.x;

	// Determine the cell we are in
	if (dx < dz)
	{
		// we are in vert1;
	}
	else
	{
		// we are in vert2
	}
	return 0.0f;
}


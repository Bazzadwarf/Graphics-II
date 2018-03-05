#include "CubeNode.h"

bool CubeNode::Initialise()
{
	BuildGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildTexture();
	return true;
}

void CubeNode::Update(FXMMATRIX & currentWorldTransformation)
{
}

void CubeNode::Render()
{
}

void CubeNode::Shutdown()
{
}

void CubeNode::BuildGeometryBuffers()
{
	float positiveX = (_size.x / 2);
	float negativeX = (_size.x / -2);
	float positiveY = (_size.y / 2);
	float negativeY = (_size.y / -2);
	float positiveZ = (_size.z / 2);
	float negativeZ = (_size.z / -2);
	Vertex vertices[] =
	{
		{ XMFLOAT4(negativeX, negativeY, positiveZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(positiveX, negativeY, positiveZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(negativeX, positiveY, positiveZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(positiveX, positiveY, positiveZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(negativeX, negativeY, negativeZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(negativeX, positiveY, negativeZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(positiveX, negativeY, negativeZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(positiveX, positiveY, negativeZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(negativeX, positiveY, negativeZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(negativeX, positiveY, positiveZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(positiveX, positiveY, negativeZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(positiveX, positiveY, positiveZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(negativeX, negativeY, negativeZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(positiveX, negativeY, negativeZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(negativeX, negativeY, positiveZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(positiveX, negativeY, positiveZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(positiveX, negativeY, negativeZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(positiveX, positiveY, negativeZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(positiveX, negativeY, positiveZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(positiveX, positiveY, positiveZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT4(negativeX, negativeY, negativeZ, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(negativeX, negativeY, positiveZ, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(negativeX, positiveY, negativeZ, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(negativeX, positiveY, positiveZ, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * 24;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = &vertices;

	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	UINT indices[] = {
		0, 1, 2,       // side 1
		2, 1, 3,
		4, 5, 6,       // side 2
		6, 5, 7,
		8, 9, 10,      // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * 36;
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &indices;

	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void CubeNode::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined(_DEBUG)
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	HRESULT hr = D3DCompileFromFile(L"shader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
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
	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));
	DXWindow::GetDXFramework()->GetDeviceContext()->VSSetShader(_vertexShader.Get(), 0, 0);

	// Compile pixel shader
	hr = D3DCompileFromFile(L"shader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
	DXWindow::GetDXFramework()->GetDeviceContext()->PSSetShader(_pixelShader.Get(), 0, 0);
}

void CubeNode::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
	DXWindow::GetDXFramework()->GetDeviceContext()->IASetInputLayout(_layout.Get());
}

void CubeNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(DXWindow::GetDXFramework()->GetDevice()->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void CubeNode::BuildTexture()
{
	// Note that in order to use CreateWICTextureFromFile, we 
	// need to ensure we make a call to CoInitializeEx in our 
	// Initialise method (and make the corresponding call to 
	// CoUninitialize in the Shutdown method).  Otherwise, 
	// the following call will throw an exception
	ThrowIfFailed(CreateWICTextureFromFile(DXWindow::GetDXFramework()->GetDevice().Get(),
		DXWindow::GetDXFramework()->GetDeviceContext().Get(),
	    _file,
		nullptr,
		_texture.GetAddressOf()
	));
}

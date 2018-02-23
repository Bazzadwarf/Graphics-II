#include "DXWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DXWindow * _thisWindow = NULL;

DXWindow::DXWindow()
{
}

DXWindow::DXWindow(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow)
{
	_thisWindow = this;

	_hInstance = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;

	_width = DEFAULT_WIDTH;
	_height = DEFAULT_HEIGHT;

	_eyePosition = XMFLOAT4(0.0f, 1.0f, -15.0f, 0.0f);
	_focalPointPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_upVector = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
}


DXWindow::~DXWindow()
{
}

void DXWindow::SetUpWindow()
{
	// clear out the window class for use
	ZeroMemory(&_wcex, sizeof(WNDCLASSEXW));

	// fill in the struct with the needed information
	_wcex.cbSize = sizeof(WNDCLASSEXW);
	_wcex.style = CS_HREDRAW | CS_VREDRAW;
	_wcex.lpfnWndProc = WndProc;
	_wcex.hInstance = _hInstance;
	_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	_wcex.lpszClassName = L"WindowClass1";

	// register the window class
	if (!RegisterClassExW(&_wcex))
	{
		MessageBox(0, "Unable to register window class", 0, 0);
	}

	RECT windowRect = { 0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;
	
	_hWnd = CreateWindowEx(NULL,
						   "WindowClass1",    // name of the window class
						   "Graphics 2",   // title of the window
						   WS_OVERLAPPEDWINDOW,    // window style
						   300,    // x-position of the window
						   300,    // y-position of the window
						   _width,    // width of the window
						   _height,    // height of the window
						   NULL,    // we have no parent window, NULL
						   NULL,    // we aren't using menus, NULL
						   _hInstance,    // application handle
						   NULL);    // used with multiple windows, NULL

	if (!_hWnd)
	{
		MessageBox(0, "Unable to create window", 0, 0);
	}
	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);
	ToggleRunning();
}

void DXWindow::SetUpDirectX()
{
	_thisWindow->InitialiseDirectX();
}

void DXWindow::InitialiseDirectX()
{
	if FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))
	{

	}
	if (!GetDeviceAndSwapChain())
	{

	}
	OnResize(WM_EXITSIZEMOVE);

	// Create camera and projection matrices (we will look at how the 
	// camera matrix is created from vectors later)
	XMStoreFloat4x4(&_viewTransformation, XMMatrixLookAtLH(XMLoadFloat4(&_eyePosition), XMLoadFloat4(&_focalPointPosition), XMLoadFloat4(&_upVector)));
	XMStoreFloat4x4(&_projectionTransformation, XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)_width / _height, 1.0f, 100.0f));
}

bool DXWindow::GetDeviceAndSwapChain()
{
	UINT createDeviceFlags = 0;

	// We are going to only accept a hardware driver or a WARP
	// driver
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = _width;
	swapChainDesc.BufferDesc.Height = _height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate to 0 and let DXGI determine the best option (refer to DXGI best practices)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _hWnd;
	// Start out windowed
	swapChainDesc.Windowed = true;
	// Enable multi-sampling to give smoother lines (set to 1 if performance becomes an issue)
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;

	// Loop through the driver types to determine which one is available to us
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;

	for (unsigned int driver = 0; driver < totalDriverTypes && driverType == D3D_DRIVER_TYPE_UNKNOWN; driver++)
	{
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(0,
			driverTypes[driver],
			0,
			createDeviceFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			_swapChain.GetAddressOf(),
			_device.GetAddressOf(),
			0,
			_deviceContext.GetAddressOf()
		)))

		{
			driverType = driverTypes[driver];
		}
	}
	if (driverType == D3D_DRIVER_TYPE_UNKNOWN)
	{
		// Unable to find a suitable device driver
		return false;
	}
	return true;
}

void DXWindow::OnResize(WPARAM wParam)
{
	// Update view and projection matrices to allow for the window size change
	XMStoreFloat4x4(&_viewTransformation, XMMatrixLookAtLH(XMLoadFloat4(&_eyePosition), XMLoadFloat4(&_focalPointPosition), XMLoadFloat4(&_upVector)));
	XMStoreFloat4x4(&_projectionTransformation, XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)_width / _height, 1.0f, 100.0f));

	// We only want to resize the buffers when the user has 
	// finished dragging the window to the new size.  Windows
	// sends a value of WM_EXITSIZEMOVE to WM_SIZE when the
	// resizing is complete.
	if (wParam != WM_EXITSIZEMOVE)
	{
		return;
	}
	// This will free any existing render and depth views (which
	// would be the case if the window was being resized)
	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_depthStencilBuffer = nullptr;

	ThrowIfFailed(_swapChain->ResizeBuffers(1, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Create a drawing surface for DirectX to render to
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(_device->CreateRenderTargetView(backBuffer.Get(), NULL, _renderTargetView.GetAddressOf()));

	// The depth buffer is used by DirectX to ensure
	// that pixels of closer objects are drawn over pixels of more
	// distant objects.

	// First, we need to create a texture (bitmap) for the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferTexture = { 0 };
	depthBufferTexture.Width = _width;
	depthBufferTexture.Height = _height;
	depthBufferTexture.ArraySize = 1;
	depthBufferTexture.MipLevels = 1;
	depthBufferTexture.SampleDesc.Count = 4;
	depthBufferTexture.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferTexture.Usage = D3D11_USAGE_DEFAULT;
	depthBufferTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Create the depth buffer.  
	ComPtr<ID3D11Texture2D> depthBuffer;
	ThrowIfFailed(_device->CreateTexture2D(&depthBufferTexture, NULL, depthBuffer.GetAddressOf()));
	ThrowIfFailed(_device->CreateDepthStencilView(depthBuffer.Get(), 0, _depthStencilView.GetAddressOf()));

	// Bind the render target view buffer and the depth stencil view buffer to the output-merger stage
	// of the pipeline. 
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// Specify a viewport of the required size
	D3D11_VIEWPORT viewPort;
	viewPort.Width = static_cast<float>(_width);
	viewPort.Height = static_cast<float>(_height);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &viewPort);
}

void DXWindow::ToggleRunning()
{
	_isAlive = !_isAlive;
}

void DXWindow::Run()
{
	while (this->_isAlive)
	{
		while (GetMessage(&_msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}
}

LRESULT DXWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		case WM_SIZE:
			_width = LOWORD(lParam);
			_height = HIWORD(lParam);
			OnResize(wParam);
			//Render();
			break;
		
		// this message is read when the window is closed
		case WM_DESTROY:
			// close the application entirely
			ToggleRunning();
			PostQuitMessage(0);
			break;
	
		default:
			// Handle any messages the switch statement didn't
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// this is the main message handler for the program
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_thisWindow != NULL)
	{
		return _thisWindow->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
}

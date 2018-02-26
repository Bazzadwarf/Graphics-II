#include "DXFramework.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DXFramework * _thisWindow = NULL;

DXFramework::DXFramework()
{
}

DXFramework::DXFramework(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow)
{
	//Point to itself
	_thisWindow = this;

	//Set handlers and command line pointers
	_hInstance = hInstance;
	_hPrevInstance = hPrevInstance;
	_lpCmdLine = lpCmdLine;
	_nCmdShow = nCmdShow;

	//Set base width and height of window
	_width = DEFAULT_WIDTH;
	_height = DEFAULT_HEIGHT;

	//Set the position of the camera
	_eyePosition = XMFLOAT4(0.0f, 1.0f, -15.0f, 0.0f);
	_focalPointPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_upVector = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
}


DXFramework::~DXFramework()
{
}

void DXFramework::SetUpWindow()
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
	
	//Set the size of the rectangle used for the window
	RECT windowRect = { 0, 0, static_cast<LONG>(_width), static_cast<LONG>(_height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;

	//Create window
	_hWnd = CreateWindowExW(NULL,
						   L"WindowClass1",    // name of the window class
						   L"Graphics 2",   // title of the window
						   WS_OVERLAPPEDWINDOW,    // window style
						   300,    // x-position of the window
						   300,    // y-position of the window
						   _width,    // width of the window
						   _height,    // height of the window
						   NULL,    // we have no parent window, NULL
						   NULL,    // we aren't using menus, NULL
						   _hInstance,    // application handle
						   NULL);    // used with multiple windows, NULL

	//Throw is failed
	if (!_hWnd)
	{
		MessageBox(0, "Unable to create window", 0, 0);
	}

	//Initialse DirectX components
	InitialiseDirectX();

	//Display window
	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);

	//Set the running boolean to true
	ToggleRunning();
}

void DXFramework::SetUpDirectX()
{
	//Initialise DirectX
	_thisWindow->InitialiseDirectX();
}

void DXFramework::InitialiseDirectX()
{
	//Initialise's the COM libary for use
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	
	//Gets the decessary devices
	GetDeviceAndSwapChain();

	//Call OnResize once
	OnResize(WM_EXITSIZEMOVE);

	// Create camera and projection matrices (we will look at how the 
	// camera matrix is created from vectors later)
	XMStoreFloat4x4(&_viewTransformation, XMMatrixLookAtLH(XMLoadFloat4(&_eyePosition), XMLoadFloat4(&_focalPointPosition), XMLoadFloat4(&_upVector)));
	XMStoreFloat4x4(&_projectionTransformation, XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)_width / _height, 1.0f, 100.0f));

	_sceneGraph = make_shared<SceneGraph>();
	CreateSceneGraph();
	_sceneGraph->Initialise();
}

bool DXFramework::GetDeviceAndSwapChain()
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

	//Set the ammount of feature levels
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	//Create the DirectX Swap Chain Description
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

void DXFramework::OnResize(WPARAM wParam)
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

void DXFramework::ToggleRunning()
{
	//Switch state of isAlive
	_isAlive = !_isAlive;
}

void DXFramework::Run()
{
	//Create necessarry variables
	HACCEL hAccelTable = LoadAccelerators(_hInstance, MAKEINTRESOURCE(IDC_GRAPHICS2));
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER nextTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastTime;
	bool updateFlag = true;

	// Initialise timer
	QueryPerformanceFrequency(&counterFrequency);
	DWORD msPerFrame = (DWORD)(counterFrequency.QuadPart / DEFAULT_FRAMERATE);
	double timeFactor = 1.0 / counterFrequency.QuadPart;
	QueryPerformanceCounter(&nextTime);
	lastTime = nextTime;

	_msg.message = WM_NULL;
	while (this->_isAlive)
	{
		//Check for updates
		if (updateFlag)
		{
			QueryPerformanceCounter(&currentTime);
			_timeSpan = (currentTime.QuadPart - lastTime.QuadPart) * timeFactor;
			lastTime = currentTime;
			//Update();
			updateFlag = false;
		}
		QueryPerformanceCounter(&currentTime);

		//Check for inputs or Render()
		if (currentTime.QuadPart > nextTime.QuadPart)
		{
			Render();
			nextTime.QuadPart += msPerFrame;
			if (nextTime.QuadPart < currentTime.QuadPart)
			{
				nextTime.QuadPart = currentTime.QuadPart + msPerFrame;
			}
			updateFlag = true;
		}
		else
		{
			while (GetMessage(&_msg, NULL, 0, 0) > 0)
			{	
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);
			}
		}
	}
}

void DXFramework::Render()
{
	//Clear background to black
	const float clearColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColour);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//Code of rendering the Scene Graph
	_sceneGraph->Render();

	//Throw if failed
	ThrowIfFailed(_swapChain->Present(0, 0));
}

void DXFramework::Update()
{
	UpdateSceneGraph();
	_sceneGraph->Update(XMMatrixIdentity());
}

void DXFramework::Shutdown()
{
	_sceneGraph->Shutdown();
	CoUninitialize();
	ToggleRunning();
}

LRESULT DXFramework::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		//This message is read when the window is being resized
		case WM_SIZE:
			_width = LOWORD(lParam);
			_height = HIWORD(lParam);
			OnResize(wParam);
			Render();
			break;

		// This message is read when the window is being moved arround the screen
		case WM_MOVING:
			Render();
			break;

		// this message is read when the window is closed
		case WM_DESTROY:
			// close the application entirely
			Shutdown();
			PostQuitMessage(0);
			break;
	
		default:
			// Handle any messages the switch statement didn't
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

DXFramework * DXFramework::GetDXFramework()
{
	return _thisWindow;
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
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}	
}

#pragma once
#include "Core.h"
#include <vector>
#define DEFAULT_FRAMERATE 60
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define MAX_LOADSTRING 100

class DXWindow
{
public:
	DXWindow();
	DXWindow(HINSTANCE & hInstance,
			 HINSTANCE & hPrevInstance,
			 LPSTR & lpCmdLine,
			 int & nCmdShow);
	~DXWindow();

	void SetUpWindow();
	void SetUpDirectX();
	void InitialiseDirectX();
	bool GetDeviceAndSwapChain();
	void OnResize(WPARAM wParam);
	void ToggleRunning();
	void Run();
	
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw exception();
		}
	}

private:
	HWND		 _hWnd;
	WNDCLASSEXW  _wcex;
	MSG			 _msg;

	HINSTANCE	 _hInstance;
	HINSTANCE	 _hPrevInstance;
	LPSTR		 _lpCmdLine;
	int			 _nCmdShow;

	unsigned int _width;
	unsigned int _height;
	bool		 _isAlive = false;

	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;
	ComPtr<IDXGISwapChain>			_swapChain;
	ComPtr<ID3D11Texture2D>			_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView>	_renderTargetView;
	ComPtr<ID3D11DepthStencilView>	_depthStencilView;

	D3D11_VIEWPORT					_screenViewport;

	// Our vectors and matrices.  Note that we cannot use
	// XMVECTOR and XMMATRIX for class variables since they need
	// to be aligned on 16-byte boundaries and the compiler cannot
	// guarantee this for class variables

	XMFLOAT4						_eyePosition;
	XMFLOAT4						_focalPointPosition;
	XMFLOAT4						_upVector;

	XMFLOAT4X4						_viewTransformation;
	XMFLOAT4X4						_projectionTransformation;
};


#pragma once
#include "Window.h"
#include "DirectXCore.h"
#include "SceneGraph.h"

class DXWindow : public Window
{
public:
	DXWindow(HINSTANCE & hInstance, HINSTANCE & hPrevInstance, LPSTR & lpCmdLine, int & nCmdShow) : Window(hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
	~DXWindow();

	static DXWindow * GetDXFramework();
	inline ComPtr<ID3D11Device> GetDevice() { return _device; }
	inline ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

	void InitialiseDirectX();
	void GetDeviceAndSwapChain();

	void OnResize(WPARAM wParam);
	void Run();
	void Render();
	void Update();
	void Shutdown();

	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void CreateSceneGraph() {};
	virtual void UpdateSceneGraph() {};
	inline SceneGraphPointer GetSceneGraph() { return _sceneGraph; }

private:
	double _timeSpan;

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

	SceneGraphPointer _sceneGraph;
};


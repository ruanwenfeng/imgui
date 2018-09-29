#include <Windows.h>
#include <d3d11.h>
#include <assert.h>
#include <iostream>
#include "game_timer.h"

/*
typedef struct DXGI_SWAP_CHAIN_DESC {
	DXGI_MODE_DESC BufferDesc;
	DXGI_SAMPLE_DESC SampleDesc;
	DXGI_USAGE BufferUsage;
	UINT BufferCount;
	HWND OutputWindow;
	BOOL Windowed;
	DXGI_SWAP_EFFECT SwapEffect;
	UINT Flags;
} DXGI_SWAP_CHAIN_DESC;


typedef struct DXGI_MODE_DESC {
	UINT Width; // desired back buffer width
	UINT Height; // desired back buffer height
	DXGI_RATIONAL RefreshRate; // display mode refresh rate
	DXGI_FORMAT Format; // back buffer pixel format
	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering; // display scanline mode
	DXGI_MODE_SCALING Scaling; // display scaling mode
} DXGI_MODE_DESC;


*/

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		//case WM_PAINT: // 重画窗口客户区消息处理
		//	return 0;
		case WM_DESTROY: // 撤销窗口消息处理
			PostQuitMessage(0); // 产生退出程序消息 WM_QUIT
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int do_main() {

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	HRESULT hr = D3D11CreateDevice(
		0, // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0, // no software device
		createDeviceFlags,
		0, 0, // default feature level array
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);
	if (FAILED(hr)) {
		MessageBox(0, TEXT("D3D11CreateDevice Failed."), 0, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, TEXT("Direct3D Feature Level 11 unsupported."), 0, 0);
		return false;
	}


	UINT m4xMsaaQuality;
	md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);
	
	//创建窗口
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),  
		CS_HREDRAW | CS_VREDRAW, 
		WndProc, 0L, 0L, 
		GetModuleHandle(NULL), LoadIcon(NULL,IDI_APPLICATION), 
		LoadCursor(NULL,IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, TEXT("Window Example"), NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindow(TEXT("Window Example"), TEXT("Dear Window DirectX11 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	RECT rect;
	GetClientRect(hwnd, &rect);
	auto mClientWidth = rect.right - rect.left;
	auto mClientHeight = rect.bottom - rect.top;
	//设置交换链
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth; // use window's client area dims
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bool mEnable4xMsaa = true;
	// Use 4X MSAA?
	if (mEnable4xMsaa) {
		sd.SampleDesc.Count = 4;
		// m4xMsaaQuality is returned via CheckMultisampleQualityLevels().
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = nullptr;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	// Finally got the IDXGIFactory interface.
	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	// Now, create the swap chain.
	IDXGISwapChain* mSwapChain;
	dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);

	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mEnable4xMsaa) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else {
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	md3dDevice->CreateTexture2D(
		&depthStencilDesc, // Description of texture to create.
		0,
		&mDepthStencilBuffer); // Return pointer to depth/stencil buffer.

	md3dDevice->CreateDepthStencilView(
		mDepthStencilBuffer, // Resource we want to create a view to.
		0,
		&mDepthStencilView); // Return depth/stencil view
	
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	md3dImmediateContext->RSSetViewports(1, &vp);

	MSG msg = { 0 };
	GameTimer mTimer;
	mTimer.Reset();
	while (msg.message != WM_QUIT) {
		// If there are Window messages then process them.
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else {
			mTimer.Tick();
			std::cout << mTimer.DeltaTime() << std::endl;
		}
	}
	std::cout << "total titme=\t" << mTimer.TotalTime() << std::endl;
	// Release our acquired COM interfaces (because we are done with them).
	mDepthStencilView->Release();
	mDepthStencilBuffer->Release();
	mRenderTargetView->Release();
	backBuffer->Release();
	mSwapChain->Release();
	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();
	md3dImmediateContext->Release();
	md3dDevice->Release();

	return static_cast<int>(msg.wParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	CHAR* lpCmdLine,
	int nCmdShow) {
	
	return do_main();

}

int main(int, char*[]) {
	return do_main();
}
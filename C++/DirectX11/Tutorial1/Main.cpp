#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Window.lib")

#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include "Window.h"

#define WIDTH 640
#define HEIGHT 480

void Render();
void CleanUp();

IDXGISwapChain * gSwapChain = NULL;
ID3D11Device * gDevice = NULL;
ID3D11DeviceContext * gContext = NULL;
ID3D11Texture2D * gBackBuffer = NULL;
ID3D11RenderTargetView * gRTView = NULL;
D3D_FEATURE_LEVEL gFeatureLevel = D3D_FEATURE_LEVEL_11_0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int iCmdShow) {
	Window::Initialize(hInst, WIDTH, HEIGHT, false, "DX11 Tutorial 1");

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Width = WIDTH;
	desc.BufferDesc.Height = HEIGHT;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 60;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = Window::hWnd;
	desc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	// create device and swap chain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &desc, &gSwapChain, &gDevice, &gFeatureLevel, &gContext);

	if(FAILED(hr)) {
		MessageBox(NULL, "Error initializing D3D", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	
	// create backbuffer
	hr = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&gBackBuffer);

	if(FAILED(hr)) {
		MessageBox(NULL, "Error creating back buffer", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	hr = gDevice->CreateRenderTargetView(gBackBuffer, NULL, &gRTView);
	if(FAILED(hr)) {
		MessageBox(NULL, "Error creating render target", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	gContext->OMSetRenderTargets(1, &gRTView, NULL);

	D3D11_VIEWPORT viewPort;
	viewPort.Width = WIDTH;
	viewPort.Height = HEIGHT;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	gContext->RSSetViewports(1, &viewPort);

	MSG msg;

	while(1) {
		if(PeekMessage(&msg, Window::hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			Render();
		}
	}

	CleanUp();
}

void Render() {
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	gContext->ClearRenderTargetView(gRTView, ClearColor);
	gSwapChain->Present(0, 0);
}

void CleanUp() {
	if(gContext) gContext->ClearState();

	if(gRTView) gRTView->Release();
	if(gSwapChain) gSwapChain->Release();
	if(gContext) gContext->Release();
	if(gDevice) gDevice->Release();
}
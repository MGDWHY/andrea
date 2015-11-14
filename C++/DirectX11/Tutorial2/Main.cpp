#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Window.lib")

#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include "Window.h"

#define WIDTH 640
#define HEIGHT 480

void Render();
void CleanUp();
void InitShaders();
void InitBuffers();

IDXGISwapChain * gSwapChain = NULL;
ID3D11Device * gDevice = NULL;
ID3D11DeviceContext * gContext = NULL;
ID3D11Texture2D * gBackBuffer = NULL;
ID3D11RenderTargetView * gRTView = NULL;
ID3D11VertexShader * gVS  = NULL;
ID3D11PixelShader * gPS = NULL;
ID3D11InputLayout * gLayout = NULL;
ID3D11Buffer * gVertexBuffer = NULL;

D3D_FEATURE_LEVEL gFeatureLevel = D3D_FEATURE_LEVEL_11_0;

D3D11_INPUT_ELEMENT_DESC layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct SimpleVertex {
	XMFLOAT3 Position;
};

SimpleVertex vertices[] =
{
    XMFLOAT3( 0.0f, 0.5f, 0.5f ),
    XMFLOAT3( 0.5f, -0.5f, 0.5f ),
    XMFLOAT3( -0.5f, -0.5f, 0.5f ),
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int iCmdShow) {
	Window::Initialize(hInst, WIDTH, HEIGHT, false, "DX11 Tutorial 2");

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
	
	InitShaders();
	InitBuffers();

	MSG msg;
	bool done = false;

	while(!done) {
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

	gContext->VSSetShader(gVS, NULL, 0);
	gContext->PSSetShader(gPS, NULL, 0);
	gContext->Draw(3, 0);

	gSwapChain->Present(0, 0);
}

void CleanUp() {
	if(gContext) gContext->ClearState();

	if(gRTView) gRTView->Release();
	if(gSwapChain) gSwapChain->Release();
	if(gContext) gContext->Release();
	if(gDevice) gDevice->Release();
}

void InitShaders() {
	HRESULT hr;

	// Create vertex shader
	ID3DBlob * errorBlob = NULL, * outBlob = NULL;

	hr = D3DX11CompileFromFile("shaders.fx", NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &outBlob, &errorBlob, NULL);
	if(FAILED(hr)) {
		MessageBox(NULL, "Error compiling shader", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	hr = gDevice->CreateVertexShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), NULL, &gVS);
	if(FAILED(hr)) {
		MessageBox(NULL, "Failed creating shader", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	// Create input layout

	hr = gDevice->CreateInputLayout(layout, 1, outBlob->GetBufferPointer(), outBlob->GetBufferSize(), &gLayout);
	if(FAILED(hr)) {
		MessageBox(NULL, "Failed creating layout", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	gContext->IASetInputLayout(gLayout);

	if(errorBlob) {
		errorBlob->Release();
		errorBlob = NULL;
	}
	outBlob->Release();

	hr = D3DX11CompileFromFile("shaders.fx", NULL, NULL, "PS", "ps_4_0", 0, 0, NULL,  &outBlob, &errorBlob, NULL);
	if(FAILED(hr)) {
		MessageBox(NULL, "Failed compiling shader", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	hr = gDevice->CreatePixelShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), NULL, &gPS);
	if(FAILED(hr)) {
		MessageBox(NULL, "Failed creating shader", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	if(errorBlob) {
		errorBlob->Release();
		errorBlob = NULL;
	}

	outBlob->Release();

}

void InitBuffers() {
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = sizeof(SimpleVertex) * 3;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	hr = gDevice->CreateBuffer(&desc, &data, &gVertexBuffer);
	if(FAILED(hr)) {
		MessageBox(NULL, "Failed creating vertex buffer", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	gContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &stride, &offset);
	gContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
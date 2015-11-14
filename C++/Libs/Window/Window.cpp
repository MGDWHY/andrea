#include "Window.h"

HWND Window::hWnd = NULL;
HINSTANCE Window::hInstance = NULL;
WNDCLASS Window::clazz;

bool Window::fullscreen = false;
std::vector<WindowListener*> Window::listeners = std::vector<WindowListener*>();

PIXELFORMATDESCRIPTOR Window::PFD = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,
	0,
	0,
	0,0,0,0,
	24,
	8,
	0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
};

void Window::AddWindowListener(WindowListener * listener) {
	Window::listeners.push_back(listener);
}

void Window::FireOnKeyDown(int key) {
	for(int i = 0; i < Window::listeners.size(); i++)
		Window::listeners[i]->OnKeyDown(key);
}

void Window::FireOnKeyUp(int key) {
	for(int i = 0; i < Window::listeners.size(); i++)
		Window::listeners[i]->OnKeyUp(key);
}

void Window::FireOnResize(int width, int height) {
	for(int i = 0; i < Window::listeners.size(); i++)
		Window::listeners[i]->OnResize(width, height);
}

void Window::FireOnClose() {
	for(int i = 0; i < Window::listeners.size(); i++)
		Window::listeners[i]->OnClose();
}

bool Window::Dispose()
{
	if(Window::fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if(Window::hWnd) 
		DestroyWindow(Window::hWnd);

	UnregisterClass("Wnd", Window::hInstance);

	return true;
}

bool Window::Initialize(HINSTANCE hInst, int width, int height, bool fullscreen, const char * title) {
	DWORD style, exStyle;
	
	Window::fullscreen = fullscreen;
	Window::hInstance = hInst;

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = (LONG)width;
	windowRect.bottom = (LONG)height;

	Window::clazz.hInstance = Window::hInstance;
	Window::clazz.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	Window::clazz.cbClsExtra = 0;
	Window::clazz.cbWndExtra = 0;
	Window::clazz.lpfnWndProc = (WNDPROC) WndProc;
	Window::clazz.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	Window::clazz.hCursor = LoadCursor(NULL, IDC_ARROW);
	Window::clazz.hbrBackground = NULL;
	Window::clazz.lpszMenuName = NULL;
	Window::clazz.lpszClassName = "Wnd";

	if(!RegisterClass(&Window::clazz)) {
		Window::ErrorMessage("Couldn't register the class");
		return false;
	}

	if(fullscreen)  {
		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		mode.dmPelsWidth = width;
		mode.dmPelsHeight = height;
		mode.dmDisplayFrequency = 60;
		mode.dmBitsPerPel = 32;
		mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		if(ChangeDisplaySettings(&mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			Window::ErrorMessage("Couldn't run in fullscreen mode. The game will run in windowed mode.");
			fullscreen = false;
		}
	}

	if(fullscreen) {
		style = WS_POPUP;
		exStyle = WS_EX_APPWINDOW;
		ShowCursor(FALSE);
	} else {
		style = WS_OVERLAPPEDWINDOW;
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

	if(!(Window::hWnd = CreateWindowEx(exStyle, 
		"Wnd", 
		title,  
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
		0, 0, 
		windowRect.right - windowRect.left, 
		windowRect.bottom - windowRect.top,
		NULL, 
		NULL, 
		Window::hInstance, 
		NULL))) 
	{
		Window::ErrorMessage("Couldn't create the window");
		return false;
	}

	ShowWindow(Window::hWnd, SW_SHOW);
	SetForegroundWindow(Window::hWnd);
	SetFocus(Window::hWnd);

	return true;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;

	switch(uMsg) {
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		Window::FireOnResize((int)LOWORD(lParam), (int)HIWORD(lParam));
		return 0;
	case WM_KEYDOWN:
		Window::FireOnKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		Window::FireOnKeyUp(wParam);
		return 0;
	case WM_DESTROY:
		Window::FireOnClose();
		return 0;
	case WM_ERASEBKGND:
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::ErrorMessage(char * msg) {
	MessageBox(NULL, (LPCSTR)msg, "Error", MB_ICONERROR | MB_OK);
}




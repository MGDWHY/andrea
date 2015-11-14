#include "GLWindow.h"

HGLRC GLWindow::hRC = NULL;
HDC GLWindow::hDC = NULL;
HWND GLWindow::hWnd = NULL;
HINSTANCE GLWindow::hInstance = NULL;
WNDCLASS GLWindow::clazz;

bool GLWindow::fullscreen = false;
std::vector<GLWindowListener*> GLWindow::listeners = vector<GLWindowListener*>();

PIXELFORMATDESCRIPTOR GLWindow::PFD = {
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

void GLWindow::AddWindowListener(GLWindowListener * listener) {
	GLWindow::listeners.push_back(listener);
}

void GLWindow::FireOnKeyDown(int key) {
	for(int i = 0; i < GLWindow::listeners.size(); i++)
		GLWindow::listeners[i]->OnKeyDown(key);
}

void GLWindow::FireOnKeyUp(int key) {
	for(int i = 0; i < GLWindow::listeners.size(); i++)
		GLWindow::listeners[i]->OnKeyUp(key);
}

void GLWindow::FireOnResize(int width, int height) {
	for(int i = 0; i < GLWindow::listeners.size(); i++)
		GLWindow::listeners[i]->OnResize(width, height);
}

void GLWindow::FireOnClose() {
	for(int i = 0; i < GLWindow::listeners.size(); i++)
		GLWindow::listeners[i]->OnClose();
}

bool GLWindow::Dispose()
{
	if(GLWindow::fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if(GLWindow::hRC)
		wglDeleteContext(GLWindow::hRC);
	
	if(GLWindow::hDC) 
		ReleaseDC(GLWindow::hWnd, GLWindow::hDC);

	if(GLWindow::hWnd) 
		DestroyWindow(GLWindow::hWnd);

	UnregisterClass("BSF", GLWindow::hInstance);

	return true;
}

bool GLWindow::Initialize(HINSTANCE hInst, Global::GameConfig gameConfig) {
	GLuint pixelFormat;
	DWORD style, exStyle;
	
	GLWindow::fullscreen = gameConfig.windowed == 1 ? false : true;
	GLWindow::hInstance = hInst;

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = (LONG)gameConfig.displayMode.width;
	windowRect.bottom = (LONG)gameConfig.displayMode.height;

	GLWindow::clazz.hInstance = GLWindow::hInstance;
	GLWindow::clazz.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	GLWindow::clazz.cbClsExtra = 0;
	GLWindow::clazz.cbWndExtra = 0;
	GLWindow::clazz.lpfnWndProc = (WNDPROC) WndProc;
	GLWindow::clazz.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	GLWindow::clazz.hCursor = LoadCursor(NULL, IDC_ARROW);
	GLWindow::clazz.hbrBackground = NULL;
	GLWindow::clazz.lpszMenuName = NULL;
	GLWindow::clazz.lpszClassName = "BSF";

	if(!RegisterClass(&GLWindow::clazz)) {
		GLWindow::ErrorMessage("Couldn't register the class");
		return false;
	}

	if(fullscreen)  {
		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		mode.dmPelsWidth = gameConfig.displayMode.width;
		mode.dmPelsHeight = gameConfig.displayMode.height;
		mode.dmDisplayFrequency = gameConfig.displayMode.frequency;
		mode.dmBitsPerPel = gameConfig.displayMode.bitsPerPixel;
		mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		if(ChangeDisplaySettings(&mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			GLWindow::ErrorMessage("Couldn't run in fullscreen mode. The game will run in windowed mode.");
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

	if(!(GLWindow::hWnd = CreateWindowEx(exStyle, 
		"BSF", 
		"Blue Spheres Forever",  
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
		0, 0, 
		windowRect.right - windowRect.left, 
		windowRect.bottom - windowRect.top,
		NULL, 
		NULL, 
		GLWindow::hInstance, 
		NULL))) 
	{
		GLWindow::ErrorMessage("Couldn't create the window");
		return false;
	}

	if(!(GLWindow::hDC = GetDC(GLWindow::hWnd)))
	{
		GLWindow::ErrorMessage("Couldn't create a device context");
		return false;
	}

	if(!(pixelFormat = ChoosePixelFormat(GLWindow::hDC, &(GLWindow::PFD))))
	{
		GLWindow::ErrorMessage("Couldn't choose a pixel format");
		return false;
	}

	if(!(SetPixelFormat(GLWindow::hDC, pixelFormat, &(GLWindow::PFD))))
	{
		GLWindow::ErrorMessage("Couldn't set up a pixel format");
		return false;
	}

	if(!(GLWindow::hRC = wglCreateContext(GLWindow::hDC))) 
	{
		GLWindow::ErrorMessage("Couldn't create a OpenGL context");
		return false;
	}

	if(!(wglMakeCurrent(GLWindow::hDC, GLWindow::hRC))) 
	{
		GLWindow::ErrorMessage("Couldn't activate the OpenGL Context");
		return false;
	}

	return true;
}

LRESULT CALLBACK GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch(uMsg) {
	case WM_SIZE:
		GLWindow::FireOnResize((int)LOWORD(lParam), (int)HIWORD(lParam));
		return 0;
	case WM_KEYDOWN:
		GLWindow::FireOnKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		GLWindow::FireOnKeyUp(wParam);
		return 0;
	case WM_CLOSE:
		GLWindow::FireOnClose();
		return 0;
	case WM_ERASEBKGND:
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GLWindow::ErrorMessage(char * msg) {
	MessageBox(NULL, (LPCSTR)msg, "Error", MB_ICONERROR | MB_OK);
}




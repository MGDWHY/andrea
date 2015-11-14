#pragma once

#include <Windows.h>
#include <vector>
#include <gl/glew.h>
#include "Global.h"


class GLWindowListener {
public:
	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;
	virtual void OnResize(int width, int height) = 0;
	virtual void OnClose() = 0;
};

class GLWindow
{
private:
	
	static PIXELFORMATDESCRIPTOR PFD;
	static WNDCLASS clazz;

	static bool fullscreen;

	static std::vector<GLWindowListener*> listeners;

	static void ErrorMessage(char * msg);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	
	static void FireOnKeyDown(int key);
	static void FireOnKeyUp(int key);
	static void FireOnResize(int width, int height);
	static void FireOnClose();

public:

	static HGLRC hRC;
	static HDC hDC;
	static HWND hWnd;
	static HINSTANCE hInstance;

	static void AddWindowListener(GLWindowListener * listener);
	static bool Initialize(HINSTANCE hInstance, Global::GameConfig gameConfig);
	static bool Dispose();
	
};


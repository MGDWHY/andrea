#pragma once

#include <Windows.h>
#include <vector>


class WindowListener {
public:
	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;
	virtual void OnResize(int width, int height) = 0;
	virtual void OnClose() = 0;
};

class Window
{
private:
	
	static PIXELFORMATDESCRIPTOR PFD;
	static WNDCLASS clazz;

	static bool fullscreen;

	static std::vector<WindowListener*> listeners;

	static void ErrorMessage(char * msg);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	
	static void FireOnKeyDown(int key);
	static void FireOnKeyUp(int key);
	static void FireOnResize(int width, int height);
	static void FireOnClose();

public:

	static HWND hWnd;
	static HINSTANCE hInstance;

	static void AddWindowListener(WindowListener * listener);
	static bool Initialize(HINSTANCE hInst, int width, int height, bool fullscreen, const char * title);
	static bool Dispose();
	
};


#pragma once
#include "../LowLevel/DirectX12.h"
#define WINDOW_NAME		"‚ ‚ ‚ "
#define CLASS_NAME		"App"

class Window {
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	WNDCLASSEX m_wndClass;

public:
	Window() = delete;
	Window(HINSTANCE hInstance) : m_hInstance(hInstance) {};
	~Window() {};

	HRESULT RegisterWindowClass(WNDCLASSEX *wnd);
	HRESULT CreateMainWindow();

	HWND GetHWND() { return m_hWnd; }
};
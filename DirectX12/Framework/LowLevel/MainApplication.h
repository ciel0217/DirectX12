
#pragma once
#include "Window.h"
#include <memory>

class MainApplication {
private:
	HINSTANCE m_hInstance;
	WNDCLASSEX m_WndClassWx;
	std::unique_ptr<Window> m_DxWindow;

	//デバッグ用
	int m_CountFPS;
	char m_DebugStr[2048] = "aaaa";

public:
	MainApplication();
	~MainApplication() {};
	int Run(HINSTANCE hInstance);
	
	HINSTANCE GetHInstance() { return m_hInstance; }
	void GetWndClass(WNDCLASSEX *wc) {
		*wc = m_WndClassWx;
	}

	Window* GetDxWindow() { return m_DxWindow.get(); }

private:
	HRESULT RegistWndClass();
	HRESULT CreateMainWnd();
	HRESULT Initialize();
	void Loop();
	void ReleaseApp();
	void Update();
	void Draw();

	//デバッグ用
	char* GetDebugStr(void) {
		return m_DebugStr;
	}
};
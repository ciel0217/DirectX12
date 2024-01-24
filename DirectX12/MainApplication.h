
#pragma once
#include "Framework/LowLevel/Window.h"
#include <memory>
#include "Framework/LowLevel/Dx12GraphicsDevice.h"


class MainApplication {
private:
	HINSTANCE m_hInstance;
	WNDCLASSEX m_WndClassWx;
	std::unique_ptr<Window> m_DxWindow;
	std::unique_ptr<Dx12GraphicsDevice> m_Dx12;
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

#if _DEBUG
	ComPtr<ID3D12DebugDevice> debugDevice;
#endif

};
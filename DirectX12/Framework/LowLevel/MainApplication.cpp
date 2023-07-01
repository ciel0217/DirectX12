#include "MainApplication.h"
#include <time.h>
#include "DirectX12/Framework/MemoryAllocator/TLSFAllocator.h"
#include "BufferView.h"

MainApplication *pApp = nullptr;

MainApplication::MainApplication()
{
	pApp = this;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	
	//pAppを生成
	MainApplication a;

	pApp->Run(hInstance);
}

int MainApplication::Run(HINSTANCE hInstance)
{

	m_hInstance = hInstance;
	m_DxWindow = std::make_unique<Window>(hInstance);


	if (RegistWndClass() != S_OK) {
		return -1;
	}

	if (CreateMainWnd() != S_OK) {
		return -1;
	}

	if (Initialize() != S_OK) {
		return -1;
	}

	Loop();

	ReleaseApp();


	return S_OK;
}


HRESULT MainApplication::RegistWndClass()
{
	HRESULT hr = m_DxWindow->RegisterWindowClass(&m_WndClassWx);
	return hr;
}

HRESULT MainApplication::CreateMainWnd()
{
	HRESULT hr = m_DxWindow->CreateMainWindow();
	return hr;
}

HRESULT MainApplication::Initialize()
{
	//初期化処理するやつ
	//renderとか
	TLSFAllocator * t = new TLSFAllocator(sizeof(BufferView));
	t->Init(1000);
	t->DivideMemory(100);

	return S_OK;
}

void MainApplication::Loop()
{
	srand((unsigned int)time(NULL));

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	MSG msg;

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;


	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				m_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(m_DebugStr, WINDOW_NAME);
				wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
				SetWindowText(m_DxWindow->GetHWND(), m_DebugStr);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

				//CheckScene();

				dwFrameCount++;
			}
		}
	}
}


void MainApplication::ReleaseApp()
{
	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, m_WndClassWx.hInstance);
}

void MainApplication::Update()
{
	
	
}

void MainApplication::Draw()
{

}

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
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	
	//pApp�𐶐�
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
	//����������������
	//render�Ƃ�
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

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;


	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				m_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(m_DebugStr, WINDOW_NAME);
				wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
				SetWindowText(m_DxWindow->GetHWND(), m_DebugStr);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

				//CheckScene();

				dwFrameCount++;
			}
		}
	}
}


void MainApplication::ReleaseApp()
{
	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, m_WndClassWx.hInstance);
}

void MainApplication::Update()
{
	
	
}

void MainApplication::Draw()
{

}

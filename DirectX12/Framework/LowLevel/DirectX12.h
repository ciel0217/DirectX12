#pragma once
#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#define no_init_all deprecated

#include <stdio.h>

#include <DirectXTex/DirectXTex.h>
#include <d3d12.h>
#include <DirectXTex/d3dx12.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"
#include <dxgi1_6.h>
#pragma warning(pop)

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d12.lib")		
#pragma comment (lib, "d3dcompiler.lib")	
#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxgi.lib")
//
//#if _DEBUG
//#pragma comment (lib, "DirectXTex/Header/x86/Debug/DirectXTex.lib")
//#else
//#pragma comment (lib, "DirectXTex/Header/x86/Release/DirectXTex.lib")
//#endif
//

	
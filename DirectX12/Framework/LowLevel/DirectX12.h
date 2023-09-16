#pragma once
#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ‚Ìwarning–h~
#define no_init_all deprecated

#include <stdio.h>

#include <DirectXTex/DirectXTex.h>
#include <d3d12.h>
#include <DirectXTex/d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#define DIRECTINPUT_VERSION 0x0800		// Œx‘Îˆ
#include "dinput.h"
#include "mmsystem.h"
#include <dxgi1_6.h>
#pragma warning(pop)

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

//*****************************************************************************
// ƒ‰ƒCƒuƒ‰ƒŠ‚ÌƒŠƒ“ƒN
//*****************************************************************************
#pragma comment (lib, "d3d12.lib")		
#pragma comment (lib, "d3dcompiler.lib")	
#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxgi.lib")


#if _DEBUG
#pragma comment (lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#pragma comment (lib, "assimp-vc142-mtd.lib")

#else
#pragma comment (lib, "DirectXTex/x64/Release/DirectXTex.lib")
#pragma comment (lib, "assimp-vc142-mt.lib")
#endif


	
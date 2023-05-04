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
//#include <d3dx9.h>
#include <wrl/client.h>
#define DIRECTINPUT_VERSION 0x0800		// Œx‘Îˆ
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ƒ‰ƒCƒuƒ‰ƒŠ‚ÌƒŠƒ“ƒN
//*****************************************************************************
#pragma comment (lib, "d3d12.lib")		
#pragma comment (lib, "d3dcompiler.lib")	
#pragma comment (lib, "winmm.lib")
//#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

//
//#if _DEBUG
//#pragma comment (lib, "DirectXTex/Header/x86/Debug/DirectXTex.lib")
//#else
//#pragma comment (lib, "DirectXTex/Header/x86/Release/DirectXTex.lib")
//#endif
//

#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)	
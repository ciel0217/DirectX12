#pragma once
#include "DirectX12Helper.h"
#include "../Resources/FrameResources.h"
#include "CommandContext.h"
#include "../Resources/Texture2D.h"
#include "DescriptorHeap.h"
#include "Window.h"
#include "../Resources/Shader.h"
#include "PipelineStateObject.h"
#include "RootSignature.h"

//ダブルバッファリング
constexpr UINT FRAME_COUNT = 2;

class Dx12GraphicsDevice
{
private:
	static Dx12GraphicsDevice* m_Instance;

	UINT m_FrameIndex;

	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT m_ScissorRect;

	ComPtr<ID3D12Device> m_Device;
	ComPtr<IDXGISwapChain3> m_SwapChain;

	CommandContext m_GraphicsCommandContext;
	FrameResources m_FrameResource[FRAME_COUNT];
	Texture2D m_DepthStencil;
	BufferView m_DSV;

	//Test用
	VertexShader m_VShader;
	PixelShader m_PShader;
	PipelineStateObject m_PSO;
	RootSignature m_RootSignature;

	void MoveToNextFrame();

public:
	Dx12GraphicsDevice() { if (!m_Instance)m_Instance = this; }

	static const Dx12GraphicsDevice* GetInstance() { return m_Instance; }
	BOOL Init(HWND hWND);
	void Update();
	void Render();
	BOOL Release();
};
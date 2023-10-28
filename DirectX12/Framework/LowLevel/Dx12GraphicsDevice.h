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
#include "../Resources/GpuBuffer.h"
#include "../Resources/Model.h"
#include "../../TestScene.h"

//ダブルバッファリング
constexpr UINT FRAME_COUNT = 2;

class Dx12GraphicsDevice
{
private:
	static Dx12GraphicsDevice* m_Instance;

	UINT m_FrameIndex;

	ComPtr<ID3D12Device> m_Device;
	ComPtr<IDXGISwapChain3> m_SwapChain;

	CommandContext m_GraphicsCommandContext;
	FrameResources m_FrameResource[FRAME_COUNT];
	Texture2D m_DepthStencil;
	BufferView m_DSV;
	
	TestScene* test;


	void MoveToNextFrame();

public:
	Dx12GraphicsDevice() { if (!m_Instance)m_Instance = this; }

	static Dx12GraphicsDevice* const GetInstance() { return m_Instance; }
	BOOL Init(HWND hWND);
	void Update();
	void Render();
	BOOL Release();

	const ComPtr<ID3D12Device> &GetDevice()const { return m_Device; }
	CommandContext* const GetGraphicContext(){ return &m_GraphicsCommandContext; }
	FrameResources* const GetFrameResource(UINT index) { return &m_FrameResource[index]; }
	BufferView* GetDSV() { return &m_DSV; }
	UINT GetFrameIndex() { return m_FrameIndex; }

};
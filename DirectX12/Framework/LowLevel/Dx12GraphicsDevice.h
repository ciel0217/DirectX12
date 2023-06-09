#pragma once
#include "DirectX12.h"


class Dx12GraphicsDevice
{
private:
	static Dx12GraphicsDevice* m_Instance;

	ComPtr<ID3D12Device> m_Device;
	ComPtr<IDXGISwapChain3> m_SwapChain;

public:
	Dx12GraphicsDevice() { if (!m_Instance)m_Instance = this; }

	static const Dx12GraphicsDevice* GetInstance() { return m_Instance; }
	BOOL Init();
	BOOL Release();
};
#pragma once
#include "DirectX12.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Dx12GraphicsDevice
{
private:
	static Dx12GraphicsDevice* m_Instance;

	ComPtr<ID3D12Device> m_Device;

public:
	Dx12GraphicsDevice() { if (!m_Instance)m_Instance = this; }

	static const Dx12GraphicsDevice* GetInstance() { return m_Instance; }
	BOOL Init();
	BOOL Release();
};
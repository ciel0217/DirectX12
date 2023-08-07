#pragma once

#include "DirectX12Helper.h"

class RootSignature
{
private:
	ComPtr<ID3D12RootSignature> m_RootSignature;

public:
	RootSignature(){}
	~RootSignature() { ShutDown(); }

	void Create(ComPtr<ID3D12Device> &device, std::vector<D3D12_ROOT_PARAMETER1> &parameters, D3D12_STATIC_SAMPLER_DESC* sampler = nullptr);
	void ShutDown();

	ComPtr<ID3D12RootSignature> GetRootSignature()const { return m_RootSignature; }
};
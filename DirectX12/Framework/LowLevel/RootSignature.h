#pragma once

#include "DirectX12Helper.h"
#include "../Resources/Shader.h"

class RootSignature
{
private:
	ComPtr<ID3D12RootSignature> m_RootSignature;

public:
	RootSignature(){}
	~RootSignature() { ShutDown(); }

	void Create(const ComPtr<ID3D12Device> &device, const std::vector<D3D12_ROOT_PARAMETER1> &parameters, D3D12_STATIC_SAMPLER_DESC* sampler = nullptr);
	//TODO:::::GeometryShader‚Æ‚©‚É‚à‘Î‰ž
	void Create(const ComPtr<ID3D12Device> &device, const VertexShader &vShader, const PixelShader &pShader);
	void ShutDown();

	ComPtr<ID3D12RootSignature> GetRootSignature()const { return m_RootSignature; }
};
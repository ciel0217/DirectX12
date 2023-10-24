#pragma once

#include "DirectX12Helper.h"
#include "../Resources/Shader.h"
#include <unordered_map>

struct BufferView;
struct CommandListSet;

class RootSignature
{
private:
	ComPtr<ID3D12RootSignature> m_RootSignature;
	std::unordered_map<std::string, UINT > m_DescriptorTableIndex;

public:
	RootSignature(){}
	~RootSignature() { ShutDown(); }

	void Create(const ComPtr<ID3D12Device> &device, const std::vector<D3D12_ROOT_PARAMETER1> &parameters, D3D12_STATIC_SAMPLER_DESC* sampler = nullptr);
	//TODO:::::GeometryShader�Ƃ��ɂ��Ή�
	void Create(const ComPtr<ID3D12Device> &device, const VertexShader* const vShader, const PixelShader* const pShader);
	void ShutDown();

	const ComPtr<ID3D12RootSignature>& GetRootSignature()const { return m_RootSignature; }

	void SetGraphicsRootDescriptorTable(const CommandListSet &commandListSet , const std::string name, const std::shared_ptr<BufferView> &bufferView);
};
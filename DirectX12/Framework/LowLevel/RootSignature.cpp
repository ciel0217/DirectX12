#include "RootSignature.h"

void RootSignature::Create(const ComPtr<ID3D12Device> &device, const std::vector<D3D12_ROOT_PARAMETER1>& parameters, D3D12_STATIC_SAMPLER_DESC * sampler)
{
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	rootSignatureDesc.Desc_1_1.NumParameters = static_cast<UINT>(parameters.size());
	rootSignatureDesc.Desc_1_1.pParameters = parameters.data();
	rootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
	rootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	if (sampler != nullptr) 
	{
		rootSignatureDesc.Desc_1_1.NumStaticSamplers = 1;
		rootSignatureDesc.Desc_1_1.pStaticSamplers = sampler;
	}

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, &error));
	ThrowIfFailed(device.Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_RootSignature.ReleaseAndGetAddressOf())));

}

void RootSignature::Create(const ComPtr<ID3D12Device>& device, const VertexShader & vShader, const PixelShader & pShader)
{
	std::vector<D3D12_ROOT_PARAMETER1> rootParameter;

	if (!vShader.GetShaderReflectResult().m_CBVRangeDescs.empty())
		rootParameter.push_back(vShader.GetCbvRootParameter());//DescriptorTable 1
	if (!vShader.GetShaderReflectResult().m_SRVRangeDescs.empty())
		rootParameter.push_back(vShader.GetSrvRootParameter());//DescriptorTable 2

	if (!pShader.GetShaderReflectResult().m_CBVRangeDescs.empty())
		rootParameter.push_back(pShader.GetCbvRootParameter());//DescriptorTable 3
	if (!pShader.GetShaderReflectResult().m_SRVRangeDescs.empty())
		rootParameter.push_back(pShader.GetSrvRootParameter());//DescriptorTable 4

	//とりあえず固定
	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = D3D12_MAX_MAXANISOTROPY;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;
	samplerDesc.RegisterSpace = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//TODO:::バージョンチェックしたほうがいいかもな

	Create(device, rootParameter, &samplerDesc);
}

void RootSignature::ShutDown()
{
	//ComPtrだから何も書かなくてもいい説？
}

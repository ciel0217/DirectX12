#include "RootSignature.h"

void RootSignature::Create(ComPtr<ID3D12Device> &device, std::vector<D3D12_ROOT_PARAMETER1>& parameters, D3D12_STATIC_SAMPLER_DESC * sampler)
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

void RootSignature::ShutDown()
{
	//ComPtrÇæÇ©ÇÁâΩÇ‡èëÇ©Ç»Ç≠ÇƒÇ‡Ç¢Ç¢ê‡ÅH
}

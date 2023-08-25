#include "PipelineStateObject.h"
#include "../Resources/Shader.h"
#include "RootSignature.h"

void PipelineStateObject::CreateGraphicPipeline(ComPtr<ID3D12Device>& device, RootSignature * rootSignature, const VertexShader * vertexShader, const PixelShader * pixelShader, const GeometryShader * geometryShader , const PipelineStateDesc & psoDesc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.InputLayout = { vertexShader->GetInputLayouts().data(), static_cast<UINT>(vertexShader->GetInputLayouts().size()) };
	desc.pRootSignature = rootSignature->GetRootSignature().Get();

	
	desc.VS = vertexShader->GetByteCode();
	desc.PS = pixelShader->GetByteCode();

	if(geometryShader != nullptr)
		desc.GS = geometryShader->GetByteCode();

	desc.RasterizerState = psoDesc.m_RasterizerDesc;
	desc.BlendState = psoDesc.m_BlendDesc;
	desc.DepthStencilState = psoDesc.m_DepthStencilDesc;
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = psoDesc.m_Topology;

	desc.NumRenderTargets = psoDesc.m_RenderTargetsNum;
	for (int i = 0; i < desc.NumRenderTargets; i++)
		desc.RTVFormats[i] = psoDesc.m_RTVFormat[i];

	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	IID_PPV_ARGS(m_PipelineState.ReleaseAndGetAddressOf());
	//
	ThrowIfFailed(device.Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(m_PipelineState.ReleaseAndGetAddressOf())));
}

void PipelineStateObject::ShutDown()
{
}

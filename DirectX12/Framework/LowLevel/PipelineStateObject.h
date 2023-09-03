#pragma once
#include "DirectX12Helper.h"




struct PipelineStateDesc
{
	PipelineStateDesc() {};
	PipelineStateDesc(UINT RenderTargetNum) :
		m_RasterizerDesc({}),
		m_BlendDesc({}),
		m_RenderTargetBlendDesc({}),
		m_DepthStencilDesc({}),
		m_DepthStencilOPDesc({}),
		m_Topology(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE),
		m_RenderTargetsNum(RenderTargetNum)
	{
		m_RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		m_RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		m_RasterizerDesc.FrontCounterClockwise = FALSE;
		m_RasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		m_RasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		m_RasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		m_RasterizerDesc.DepthClipEnable = TRUE;
		m_RasterizerDesc.MultisampleEnable = FALSE;
		m_RasterizerDesc.AntialiasedLineEnable = FALSE;
		m_RasterizerDesc.ForcedSampleCount = 0;
		m_RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		m_BlendDesc.AlphaToCoverageEnable = FALSE;
		m_BlendDesc.IndependentBlendEnable = FALSE;

		m_RenderTargetBlendDesc.BlendEnable = FALSE;
		m_RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		m_RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		m_RenderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
		m_RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		m_RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
		m_RenderTargetBlendDesc.LogicOpEnable = FALSE;
		m_RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
		m_RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;

		for (UINT i = 0; i < m_RenderTargetsNum; ++i) {
			m_BlendDesc.RenderTarget[i] = m_RenderTargetBlendDesc;
			m_RTVFormat[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		m_DepthStencilDesc.DepthEnable = FALSE;
		m_DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		m_DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_DepthStencilDesc.StencilEnable = FALSE;
		m_DepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		m_DepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

		m_DepthStencilOPDesc.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		m_DepthStencilOPDesc.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		m_DepthStencilOPDesc.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		m_DepthStencilOPDesc.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

		m_DepthStencilDesc.FrontFace = m_DepthStencilOPDesc;
		m_DepthStencilDesc.BackFace = m_DepthStencilOPDesc;
	}
	D3D12_RASTERIZER_DESC m_RasterizerDesc;
	D3D12_BLEND_DESC m_BlendDesc;
	D3D12_RENDER_TARGET_BLEND_DESC m_RenderTargetBlendDesc;
	D3D12_DEPTH_STENCIL_DESC m_DepthStencilDesc;
	D3D12_DEPTH_STENCILOP_DESC m_DepthStencilOPDesc;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE m_Topology;
	UINT m_RenderTargetsNum;
	DXGI_FORMAT m_RTVFormat[8];
};



class RootSignature;
class VertexShader;
class PixelShader;
class GeometryShader;


class PipelineStateObject
{
private:
	ComPtr<ID3D12PipelineState> m_PipelineState;

public:
	PipelineStateObject(){}
	~PipelineStateObject() { ShutDown(); }

	void CreateGraphicPipeline(ComPtr<ID3D12Device> &device, RootSignature* rootSignature, 
		const VertexShader* vertexShader, const PixelShader* pixelShader, const GeometryShader* geometryShader = nullptr,
		const PipelineStateDesc &psoDesc = PipelineStateDesc(8));

	//–¢ŽÀ‘•
	void CreateComputePipeline();

	void ShutDown();

	const ComPtr<ID3D12PipelineState>& GetPipelineState() const{ return m_PipelineState; }
};
#pragma once
#include "DirectX12Helper.h"




struct PipelineStateDesc
{
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
		const VertexShader* vertexShader, const PixelShader* pixelShader, const GeometryShader* geometryShader,
		const PipelineStateDesc &psoDesc);

	//–¢ŽÀ‘•
	void CreateComputePipeline();

	void ShutDown();
};
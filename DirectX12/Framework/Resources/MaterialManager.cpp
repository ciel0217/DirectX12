#include "MaterialManager.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/BufferView.h"

MaterialManager* MaterialManager::m_Instance = nullptr;

void MaterialManager::CalcMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue, PipelineStateType pipeline_type)
{
	//すでに生成済み
	if (m_Materials.count(material_name))
		return;

	//マテリアルは違うけど、使ってるshaderが全部一緒のマテリアルがある場合、shaderやrootSignatureは生成しない
	for (auto material : m_Materials)
	{
		if (material.second.get()->GetVertexShaderName() == vertex_name && material.second.get()->GetPixelShaderName() == pixel_name)
		{
			std::shared_ptr<RenderSet> renderSet = material.second->GetRenderSet();


			std::shared_ptr<VertexShader> v = renderSet->vShader;
			std::shared_ptr<PixelShader> p = renderSet->pShader;
			std::shared_ptr<RootSignature> r = renderSet->rootSignature;
			std::shared_ptr<PipelineStateObject> pso = renderSet->pipelineStateObj;

			m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, new RenderSet(v, p, r, pso), render_queue));

			return;
		}
	}


	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	VertexShader* v = new VertexShader();
	v->Create(vertex_name);

	PixelShader* p = new PixelShader();
	p->Create(pixel_name);

	RootSignature* signature = new RootSignature();
	signature->Create(device, v, p);

	PipelineStateObject* pso = new PipelineStateObject();
	PipelineStateDesc desc; 
	switch (pipeline_type)
	{
	case e3DPipeline:
		desc = PipelineStateDesc(8);
		break;
	case e2DPipeline:
		desc = PipelineStateDesc(1);
		desc.m_DepthStencilDesc.DepthEnable = false;
		desc.m_RasterizerDesc.DepthClipEnable = false;
		desc.m_DSVFormat = DXGI_FORMAT_UNKNOWN;
		break;
	default:
		break;
	}
	pso->CreateGraphicPipeline(device, signature, v, p, nullptr, desc);

	m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, new RenderSet(v, p, signature, pso), render_queue));

	return;
}

//textureあり
std::shared_ptr<Material> MaterialManager::CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name,
	int render_queue, std::unordered_map<std::string, std::shared_ptr<TextureSet>> textures, PipelineStateType pipeline_type)
{
	CalcMaterial(material_name, vertex_name, pixel_name, render_queue, pipeline_type);

	m_Materials[material_name]->SetTextures(textures);

	return m_Materials[material_name];
}

//textureなし
std::shared_ptr<Material> MaterialManager::CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue, 
	PipelineStateType pipeline_type)
{
	CalcMaterial(material_name, vertex_name, pixel_name, render_queue, pipeline_type);

	return m_Materials[material_name];
}

#include "MaterialManager.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/BufferView.h"

MaterialManager* MaterialManager::m_Instance = nullptr;

std::shared_ptr<Material> MaterialManager::CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue)
{
	//���łɐ����ς�
	if (m_Materials.count(material_name))
		return m_Materials[material_name];

	//�}�e���A���͈Ⴄ���ǁA�g���Ă�shader���S���ꏏ�̃}�e���A��������ꍇ�Ashader��rootSignature�͐������Ȃ�
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

			return m_Materials[material_name];
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
	pso->CreateGraphicPipeline(device, signature, v, p);

	m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, new RenderSet(v, p, signature, pso), render_queue));

	return m_Materials[material_name];
}

#include "MaterialManager.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"

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

			
			VertexShader* v = renderSet->vShader;
			PixelShader* p = renderSet->pShader;
			RootSignature* r = renderSet->rootSignature;
			PipelineStateObject* pso = renderSet->pipelineStateObj;

			m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, new RenderSet(v, p, r, pso), render_queue));

			return m_Materials[material_name];
		}
	}
	

	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	VertexShader v;
	v.Create(vertex_name);

	PixelShader p;
	p.Create(pixel_name);

	RootSignature signature;
	signature.Create(device, &v, &p);

	PipelineStateObject pso;
	pso.CreateGraphicPipeline(device, &signature, &v, &p);

	m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, new RenderSet(&v, &p, &signature, &pso), render_queue));

	return m_Materials[material_name];
}

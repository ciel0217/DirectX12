#include "MaterialManager.h"
#include "Material.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"

MaterialManager* MaterialManager::m_Instance = nullptr;

Material * MaterialManager::CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue)
{
	if (m_Materials.count(material_name))
		return m_Materials[material_name].get();//TODO:::shared_ptr‚²‚Æ“n‚µ‚½•û‚ª‚¢‚¢‚Ì‚©...H

	m_Materials[material_name].reset(new Material(material_name, vertex_name, pixel_name, render_queue));

	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	VertexShader v;
	v.Create(vertex_name);

	PixelShader p;
	p.Create(pixel_name);

	RootSignature signature;
	signature.Create(device, &v, &p);

	PipelineStateObject pso;
	pso.CreateGraphicPipeline(device, &signature, &v, &p);

	m_RenderSets[material_name].reset(new RenderSet(&v, &p, &signature, &pso));
	

	return m_Materials[material_name].get();
}

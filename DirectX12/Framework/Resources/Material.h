#pragma once
#include <string>
#include <unordered_map>


class Material;
class VertexShader;
class PixelShader;
class RootSignature;
class PipelineStateObject;


struct RenderSet
{
	VertexShader* vShader;
	PixelShader* pShader;
	RootSignature* rootSignature;
	PipelineStateObject* pipelineStateObj;

	RenderSet() = delete;
	RenderSet(VertexShader* v, PixelShader* p,
		RootSignature* signature, PipelineStateObject* pso) :vShader(v), pShader(p), rootSignature(signature), pipelineStateObj(pso) {}
	~RenderSet() { delete vShader; delete pShader; delete rootSignature; delete pipelineStateObj; }
};

class Material
{
private:
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	std::string m_MaterialName;
	std::shared_ptr<RenderSet> m_RenderSet;

	int m_RenderQueue;

public:
	Material() = delete;
	Material(std::string material_name, std::string vertex_name, std::string pixel_name, RenderSet* render_set, int render_queue) : 
		m_MaterialName(material_name), m_VertexShaderName(m_VertexShaderName), m_PixelShaderName(pixel_name), m_RenderSet(render_set), m_RenderQueue(render_queue){}
	~Material() {};

	std::string GetMaterialName() { return m_MaterialName; }
	std::string GetVertexShaderName() { return m_VertexShaderName; }
	std::string GetPixelShaderName() { return m_PixelShaderName; }
	int GetRenderQueue() { return m_RenderQueue; }
};
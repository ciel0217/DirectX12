#pragma once
#include <string>
#include <unordered_map>


class VertexShader;
class PixelShader;
class RootSignature;
class PipelineStateObject;
class ConstantBuffer;
struct BufferView;

struct RenderSet
{
	std::shared_ptr<VertexShader> vShader;
	std::shared_ptr<PixelShader> pShader;
	std::shared_ptr<RootSignature> rootSignature;
	std::shared_ptr<PipelineStateObject> pipelineStateObj;

	RenderSet() = delete;
	RenderSet(VertexShader* v, PixelShader* p,
		RootSignature* signature, PipelineStateObject* pso) 
	{
		vShader.reset(v); pShader.reset(p); rootSignature.reset(signature); pipelineStateObj.reset(pso);
	}

	RenderSet(std::shared_ptr<VertexShader> v, std::shared_ptr<PixelShader> p,
		std::shared_ptr<RootSignature> signature, std::shared_ptr<PipelineStateObject> pso)
	{
		vShader = v; pShader = p; rootSignature = signature; pipelineStateObj = pso;
	}

	~RenderSet() { }
};

struct CBufferSet
{
	std::unique_ptr<ConstantBuffer> constantBuffer;
	std::unique_ptr<BufferView> constantBufferView;

	CBufferSet(ConstantBuffer* buffer, BufferView* bufferView) : constantBuffer(buffer), constantBufferView(bufferView){}
};

class Material
{
private:


	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	std::string m_MaterialName;
	std::shared_ptr<RenderSet> m_RenderSet;
	std::unordered_map<std::string, std::shared_ptr<CBufferSet>> m_CBufferSet;

	int m_RenderQueue;

	void CalcConstantBuffer();

public:
	Material() = delete;
	Material(std::string material_name, std::string vertex_name, std::string pixel_name, RenderSet* render_set, int render_queue) : 
		m_MaterialName(material_name), m_VertexShaderName(vertex_name), m_PixelShaderName(pixel_name),
		m_RenderSet(render_set), m_RenderQueue(render_queue)
	{
		CalcConstantBuffer();
	}
	
	~Material() {};

	std::string GetMaterialName() { return m_MaterialName; }
	std::string GetVertexShaderName() { return m_VertexShaderName; }
	std::string GetPixelShaderName() { return m_PixelShaderName; }
	int GetRenderQueue() { return m_RenderQueue; }
	std::shared_ptr<RenderSet> GetRenderSet() { return m_RenderSet; }
};
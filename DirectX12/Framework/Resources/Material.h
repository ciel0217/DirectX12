#pragma once
#include <string>
#include <unordered_map>
#include "GpuBuffer.h"
#include "Shader.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/BufferView.h"
#include "Texture2D.h"

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
	std::shared_ptr<ConstantBuffer> constantBuffer;
	std::shared_ptr<BufferView> constantBufferView;

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
	std::unordered_map<std::string, std::shared_ptr<TextureSet>> m_Textures;

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
	std::unordered_map<std::string, std::shared_ptr<TextureSet>> GetTextures() { return m_Textures; }
	std::unordered_map<std::string, std::shared_ptr<CBufferSet>> GetCBuffers() { return m_CBufferSet; }

	void SetTextures(std::unordered_map<std::string, std::shared_ptr<TextureSet>> textures) { m_Textures = textures; }
	void SetTextureByTextureType(std::string name, TextureSet* textureSet ) { m_Textures[name].reset(textureSet); }
	void SetCBufferData(std::string name, void* data, UINT size) { m_CBufferSet[name]->constantBuffer->WriteData(data, size); }

	bool HasTextureByTextureType(std::string name) { return m_Textures.count(name); }

};
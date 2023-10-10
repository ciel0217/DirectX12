#pragma once
#include <unordered_map>
#include <memory>

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
	RootSignature* signature, PipelineStateObject* pso):vShader(v), pShader(p), rootSignature(signature), pipelineStateObj(pso){}
	~RenderSet() { delete vShader; delete pShader; delete rootSignature; delete pipelineStateObj; }
};


//マテリアル管理とRenderSet管理
class MaterialManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
	std::unordered_map<std::string, std::shared_ptr<RenderSet>> m_RenderSets;

	static MaterialManager* m_Instance;

	MaterialManager() = default;
	~MaterialManager() = default;

public:
	//コピー禁止
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;
	//ムーブ禁止
	MaterialManager(MaterialManager&&) = delete;
	MaterialManager& operator=(MaterialManager&&) = delete;
	
	static MaterialManager* GetInstance() { return m_Instance; }
	static void Create() { if (!m_Instance)m_Instance = new MaterialManager(); }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	Material* CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue);

};
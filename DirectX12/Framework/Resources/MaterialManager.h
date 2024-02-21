#pragma once
#include <unordered_map>
#include <memory>
#include "Material.h"


//TODO:::���O���v�������΂Ȃ�...Depth�g�����ǂ����ȂǂŐ؂�ւ���
enum PipelineStateType
{
	e3DPipeline,
	e2DPipeline,
};

//�}�e���A���Ǘ�
class MaterialManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;

	static MaterialManager* m_Instance;

	MaterialManager() = default;
	~MaterialManager() = default;

	void CalcMaterial(std::string material_name, std::string vertex_name, std::string pixel_name,
		int render_queue, PipelineStateType pipeline_type);

public:

	static const int OPACITY_RENDER_QUEUE = 3000;
	static const int TRANSPARENT_RENDER_QUEUE = 7500;
	static const int D2_RENDER_QUEUE = 10000;

	

	//�R�s�[�֎~
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;
	//���[�u�֎~
	MaterialManager(MaterialManager&&) = delete;
	MaterialManager& operator=(MaterialManager&&) = delete;
	
	static MaterialManager* GetInstance() { return m_Instance; }
	static void Create() { if (!m_Instance)m_Instance = new MaterialManager();  }
	static void Destruct() { delete m_Instance; m_Instance = nullptr; }

	std::shared_ptr<Material> CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, 
		int render_queue, std::unordered_map<std::string, std::shared_ptr<TextureSet>> textures, PipelineStateType pipeline_type = e3DPipeline);

	std::shared_ptr<Material> CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name,
		int render_queue, PipelineStateType pipeline_type = e3DPipeline);
};
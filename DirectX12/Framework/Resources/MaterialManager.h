#pragma once
#include <unordered_map>
#include <memory>
#include "Material.h"

//�}�e���A���Ǘ�
class MaterialManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;

	static MaterialManager* m_Instance;

	MaterialManager() = default;
	~MaterialManager() = default;

public:
	//�R�s�[�֎~
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;
	//���[�u�֎~
	MaterialManager(MaterialManager&&) = delete;
	MaterialManager& operator=(MaterialManager&&) = delete;
	
	static MaterialManager* GetInstance() { return m_Instance; }
	static void Create() { if (!m_Instance)m_Instance = new MaterialManager(); }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	std::shared_ptr<Material> CreateMaterial(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue);
};
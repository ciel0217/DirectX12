#pragma once
#include <string>

class Material
{
private:
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	std::string m_MaterialName;

	int m_RenderQueue;

public:
	Material() = delete;
	Material(std::string material_name, std::string vertex_name, std::string pixel_name, int render_queue) : 
		m_MaterialName(material_name), m_VertexShaderName(m_VertexShaderName), m_PixelShaderName(pixel_name), m_RenderQueue(render_queue){}
	~Material() {};

	std::string GetMaterialName() { return m_MaterialName; }
	std::string GetVertexShaderName() { return m_VertexShaderName; }
	std::string GetPixelShaderName() { return m_PixelShaderName; }
	int GetRenderQueue() { return m_RenderQueue; }
};
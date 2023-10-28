#pragma once
#include "GpuBuffer.h"
#include "Material.h"

struct Mesh
{
	std::vector<VERTEX_3D> m_Vertices;
	std::vector<UINT> m_Indices;
};

struct aiScene;
struct aiMaterial;

class Model
{
private:

	const std::string DEFAULT_VERTEX_SHADER_NAME = "FrameWork/Shader/TestVertex.cso";

	const std::string DEFAULT_PIXEL_SHADER_NAME = "FrameWork/Shader/TestPixel.cso";


	const std::string m_Name;
	std::vector<VertexBuffer> m_VertexBuffer;
	std::vector<IndexBuffer> m_IndexBuffer;
	std::vector<UINT> m_IndexNum;
	
	std::vector<std::shared_ptr<Material>> m_Materials;


	void CreateVertexBuffer(const std::vector<Mesh> &mesh);
	void CreateIndexBuffer(const std::vector<Mesh> &mesh);
	
	void CreateTexture(TextureType type, aiMaterial* material, std::unordered_map<TextureType, std::shared_ptr<BufferView>> &textures);

	void LoadMesh(const aiScene* scene);
	void LoadTexture(const aiScene* scene);
	
public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	void LoadModel();
	

	std::vector<VertexBuffer> const &GetVertexBuffer() const{ return m_VertexBuffer; }
	std::vector<IndexBuffer> const &GetIndexBuffer() const{ return m_IndexBuffer; }
	std::vector<UINT> const &GetIndexNum()const { return m_IndexNum; }
	std::vector<std::shared_ptr<Material>> const &GetMaterials() { return m_Materials; }
};
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

enum TextureType
{
	eDiffuse = 0,
	eNormal,
	eShiness,
	eSpecular,
	eRoughMetaSpe
};



class Model
{
private:

	const std::string DEFAULT_VERTEX_SHADER_NAME = "FrameWork/Shader/DefaultGBufferVS.cso";

	const std::string DEFAULT_PIXEL_SHADER_NAME = "FrameWork/Shader/DefaultGBufferPS.cso";


	const std::string m_Name;
	std::vector<VertexBuffer> m_VertexBuffer;
	std::vector<IndexBuffer> m_IndexBuffer;
	std::vector<UINT> m_IndexNum;
	std::vector<std::shared_ptr<Material>> m_Materials;


	void CreateVertexBuffer(const std::vector<Mesh> &mesh);
	void CreateIndexBuffer(const std::vector<Mesh> &mesh);
	
	void CreateTexture(TextureType type, aiMaterial* material, std::unordered_map<std::string, std::shared_ptr<TextureSet>> &textures);

	void LoadMesh(const aiScene* scene, bool inverseU, bool inverseV);
	void LoadTexture(const aiScene* scene);
	
public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	 std::vector<std::shared_ptr<Material>> LoadModel(bool inverseU, bool inverseV);
	

	std::vector<VertexBuffer> const &GetVertexBuffer() const{ return m_VertexBuffer; }
	std::vector<IndexBuffer> const &GetIndexBuffer() const{ return m_IndexBuffer; }
	std::vector<UINT> const &GetIndexNum()const { return m_IndexNum; }
	void LoadAndSetTexture(TextureType type, std::string fileName);
	//std::vector<std::shared_ptr<Material>> const &GetMaterials() { return m_Materials; }
};
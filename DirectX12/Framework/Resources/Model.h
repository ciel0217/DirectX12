#pragma once
#include "GpuBuffer.h"

struct Mesh
{
	std::vector<VERTEX_3D> m_Vertices;
	std::vector<UINT> m_Indices;
};

class Model
{
private:
	const std::string m_Name;
	std::vector<VertexBuffer> m_VertexBuffer;
	std::vector<IndexBuffer> m_IndexBuffer;
	std::vector<UINT> m_IndexNum;
	//TODO:::�}�e���A���Ȃǒǉ�



	void CreateVertexBuffer(const std::vector<Mesh> &mesh);
	void CreateIndexBuffer(const std::vector<Mesh> &mesh);

public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	void LoadModel();
	

	const std::vector<VertexBuffer> &GetVertexBuffer() const{ return m_VertexBuffer; }
	const std::vector<IndexBuffer> &GetIndexBuffer() const{ return m_IndexBuffer; }
	const std::vector<UINT> &GetIndexNum()const { return m_IndexNum; }
};
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
	//TODO:::ƒ}ƒeƒŠƒAƒ‹‚È‚Ç’Ç‰Á



	void CreateVertexBuffer(const std::vector<Mesh> &mesh);
	void CreateIndexBuffer(const std::vector<Mesh> &mesh);

public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	void LoadModel();
	

	const std::vector<VertexBuffer> &GetVertexBuffer() const{ return m_VertexBuffer; }
	const std::vector<IndexBuffer> &GetIndexBuffer() const{ return m_IndexBuffer; }
};
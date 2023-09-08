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
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	//TODO:::ƒ}ƒeƒŠƒAƒ‹‚È‚Ç’Ç‰Á

public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	void LoadModel();

	const VertexBuffer &GetVertexBuffer() const{ return m_VertexBuffer; }
	const IndexBuffer &GetIndexBuffer() const{ return m_IndexBuffer; }
};
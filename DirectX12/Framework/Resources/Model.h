#pragma once
#include "GpuBuffer.h"


class Model
{
private:
	const std::string m_Name;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	//TODO:::�}�e���A���Ȃǒǉ�

public:
	Model() = delete;
	Model(std::string name):m_Name(name){}
	virtual ~Model(){}

	void LoadModel();

	const VertexBuffer &GetVertexBuffer() const{ return m_VertexBuffer; }
	const IndexBuffer &GetIndexBuffer() const{ return m_IndexBuffer; }
};
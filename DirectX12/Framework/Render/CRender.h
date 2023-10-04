#pragma once

class VertexShader;
class PixelShader;
class RootSignature;
class PipelineStateObject;

class CRender
{
protected:
	VertexShader* m_VertexShader;
	PixelShader* m_PixelShader;
	RootSignature* m_RootSignature;
	PipelineStateObject* m_PSO;

public:
	CRender(){}
	virtual ~CRender(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw() = 0;
};
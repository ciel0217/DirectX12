#pragma once
#include "../LowLevel/DirectX12Helper.h"


class Shader
{
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_Shader;

public:
	Shader(){}
	virtual ~Shader() {}

	virtual void Create() = 0;

	D3D12_SHADER_BYTECODE GetByteCode()const;
};

class VertexShader : public Shader
{
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;

public:
	VertexShader() {}
	~VertexShader() {}

	void Create()override;


	std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayouts()const { return m_InputLayouts; }
};

class PixelShader : public Shader
{
public:
	PixelShader(){}
	~PixelShader(){}

	void Create()override;
};

class GeometryShader : public Shader
{
public:
	GeometryShader() {}
	~GeometryShader(){}

	void Create()override;
};
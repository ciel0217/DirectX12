#pragma once
#include "../LowLevel/DirectX12Helper.h"

//TODO:::ルート32Bit定数についての処理を追加

struct ShaderObject
{
	unsigned char * m_Ptr;//先頭ポインタ
	UINT m_Size;
};

struct ShaderReflectResult
{
	std::vector<D3D12_DESCRIPTOR_RANGE1> m_CBVRangeDescs;
	std::vector<D3D12_DESCRIPTOR_RANGE1> m_SRVRangeDescs;
};

class Shader
{
protected:
	//ComPtr<ID3DBlob> m_Shader;//ランタイム中にコンパイルするときはこっち
	ShaderObject m_Shader;//コンパイル済み(cso)のときはこっち
	
	ComPtr<ID3D12ShaderReflection> m_ShaderRef;
	std::string m_FileName;
	ShaderReflectResult m_ShaderRefResult;

	//SRVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter(const D3D12_SHADER_VISIBILITY visibility)const;
	//CBVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter(const D3D12_SHADER_VISIBILITY visibility)const;
	//TODO::::Root32BitConstant用のCBVのRootParameterを取得
	

public:
	Shader(){}
	virtual ~Shader() {}

	virtual void Create(std::string fileName) = 0;

	void LoadShaderFile();
	void CalcShaderReflection(const D3D12_SHADER_BYTECODE& byteCode);
	D3D12_SHADER_BYTECODE GetByteCode()const;

	const ShaderReflectResult& GetShaderReflectResult()const { return m_ShaderRefResult; }

};

class VertexShader : public Shader
{
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;

	void CalcInputLayout();
	DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);

public:
	VertexShader() {}
	~VertexShader() {}

	void Create(std::string fileName)override;

	//SRVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter()const { return Shader::GetSrvRootParameter(D3D12_SHADER_VISIBILITY_VERTEX); }
	//CBVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter()const { return Shader::GetCbvRootParameter(D3D12_SHADER_VISIBILITY_VERTEX); }

	const std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayouts()const { return m_InputLayouts; }
};

class PixelShader : public Shader
{
public:
	PixelShader(){}
	~PixelShader(){}

	void Create(std::string fileName)override;

	//SRVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter()const { return Shader::GetSrvRootParameter(D3D12_SHADER_VISIBILITY_PIXEL); }
	//CBVのRootParameterを取得
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter()const { return Shader::GetCbvRootParameter(D3D12_SHADER_VISIBILITY_PIXEL); }

};

class GeometryShader : public Shader
{
public:
	GeometryShader() {}
	~GeometryShader(){}

	void Create(std::string fileName)override;
};
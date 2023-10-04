#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include <unordered_map>

//TODO:::���[�g32Bit�萔�ɂ��Ă̏�����ǉ�

struct ShaderObject
{
	unsigned char * m_Ptr;//�擪�|�C���^
	UINT m_Size;
};


struct ShaderReflectResult
{
	std::unordered_map<std::string, D3D12_DESCRIPTOR_RANGE1> m_CBVRangeDescs;
	std::unordered_map<std::string, D3D12_DESCRIPTOR_RANGE1> m_SRVRangeDescs;
};

class Shader
{
protected:
	//ComPtr<ID3DBlob> m_Shader;//�����^�C�����ɃR���p�C������Ƃ��͂�����
	ShaderObject m_Shader;//�R���p�C���ς�(cso)�̂Ƃ��͂�����
	
	ComPtr<ID3D12ShaderReflection> m_ShaderRef;
	std::string m_FileName;
	ShaderReflectResult m_ShaderRefResult;

	//SRV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter(const D3D12_SHADER_VISIBILITY visibility, const std::string name)const;
	//CBV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter(const D3D12_SHADER_VISIBILITY visibility, const std::string name)const;
	//TODO::::Root32BitConstant�p��CBV��RootParameter���擾
	

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

	//SRV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter(const std::string name)const { return Shader::GetSrvRootParameter(D3D12_SHADER_VISIBILITY_VERTEX, name); }
	//CBV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter(const std::string name)const { return Shader::GetCbvRootParameter(D3D12_SHADER_VISIBILITY_VERTEX, name); }

	const std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayouts()const { return m_InputLayouts; }
};

class PixelShader : public Shader
{
public:
	PixelShader(){}
	~PixelShader(){}

	void Create(std::string fileName)override;

	//SRV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetSrvRootParameter(const std::string name)const { return Shader::GetSrvRootParameter(D3D12_SHADER_VISIBILITY_PIXEL, name); }
	//CBV��RootParameter���擾
	D3D12_ROOT_PARAMETER1 GetCbvRootParameter(const std::string name)const { return Shader::GetCbvRootParameter(D3D12_SHADER_VISIBILITY_PIXEL, name); }

};

class GeometryShader : public Shader
{
public:
	GeometryShader() {}
	~GeometryShader(){}

	void Create(std::string fileName)override;
};
#include "Shader.h"
#include <io.h>

void Shader::LoadShaderFile()
{
	// コンパイル済みシェーダーの読み込み.
	FILE* fp = nullptr;
	fopen_s(&fp, m_FileName.c_str(), "rb");
	
	if (!fp)
		return ;

	fseek(fp, 0, SEEK_END);
	m_Shader.m_Size = _filelength(_fileno(fp));
	rewind(fp);
	m_Shader.m_Ptr = new unsigned char[m_Shader.m_Size];

	fread(m_Shader.m_Ptr, m_Shader.m_Size, 1, fp);
	fclose(fp);
}

//TODO:::引数要らない説
void Shader::CalcShaderReflection(const D3D12_SHADER_BYTECODE & byteCode)
{
	ThrowIfFailed(D3DReflect(m_Shader.m_Ptr, m_Shader.m_Size, IID_ID3D12ShaderReflection, reinterpret_cast<void**>(m_ShaderRef.ReleaseAndGetAddressOf())));

	D3D12_SHADER_DESC shaderDesc = {};
	m_ShaderRef.Get()->GetDesc(&shaderDesc);

	//TODO:::ここ無くていいかも要検証(実行速度)
	//まずはこのシェーダーに含まれるリソースの数を調べる
	UINT textureCount = 0;
	UINT constantBufferCount = 0;

	std::vector<D3D12_SHADER_BUFFER_DESC> cBufferDescs;
	cBufferDescs.resize(shaderDesc.ConstantBuffers);

	for (UINT i = 0; i < shaderDesc.ConstantBuffers; i++)
	{
		ID3D12ShaderReflectionConstantBuffer* cBuffer = m_ShaderRef->GetConstantBufferByIndex(i);
		
		D3D12_SHADER_BUFFER_DESC cBufferDesc = {};
		cBuffer->GetDesc(&cBufferDesc);
		cBufferDescs[i] = cBufferDesc;
	}
	/*for (UINT i = 0; i < shaderDesc.BoundResources; ++i) {
		D3D12_SHADER_INPUT_BIND_DESC bindDesc = {};
		m_ShaderRef.Get()->GetResourceBindingDesc(i, &bindDesc);

		switch (bindDesc.Type) {
		case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
			constantBufferCount++;
			break;

		case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
			textureCount++;
			break;
		}
	}*/

	//m_ShaderRefResult.m_CBVRangeDescs.resize(shaderDesc.ConstantBuffers);//どっちでもいいのかな
	//m_ShaderRefResult.m_CBVRangeDescs.resize(constantBufferCount);
	//m_ShaderRefResult.m_SRVRangeDescs.resize(textureCount);

	UINT cbCount = 0;
	UINT srvCount = 0;
	for (UINT i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC bindDesc = {};
		m_ShaderRef.Get()->GetResourceBindingDesc(i, &bindDesc);
		
		switch (bindDesc.Type)
		{
		case D3D_SIT_CBUFFER:
		
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.NumDescriptors = 1;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.BaseShaderRegister = bindDesc.BindPoint;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.RegisterSpace = 0;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			m_ShaderRefResult.m_CBVRangeDescs[bindDesc.Name].desc = cBufferDescs[cbCount];
			cbCount++;
			
			break;
		case D3D_SIT_TEXTURE:
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].NumDescriptors = 1;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].BaseShaderRegister = bindDesc.BindPoint;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].RegisterSpace = 0;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			
			srvCount++;

			break;
		//StructuredBuffer使うときに使う
		case D3D_SIT_STRUCTURED:
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].NumDescriptors = 1;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].BaseShaderRegister = bindDesc.BindPoint;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].RegisterSpace = 0;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
			m_ShaderRefResult.m_SRVRangeDescs[bindDesc.Name].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			srvCount++;
			break;
		default:
			break;
		}
	}
}

D3D12_SHADER_BYTECODE Shader::GetByteCode()const
{
	D3D12_SHADER_BYTECODE byteCode = {};
	byteCode.BytecodeLength = m_Shader.m_Size;
	byteCode.pShaderBytecode = m_Shader.m_Ptr;
	return byteCode;
}

D3D12_ROOT_PARAMETER1 Shader::GetSrvRootParameter(const D3D12_SHADER_VISIBILITY visibility, const std::string name)const
{
	if (m_ShaderRefResult.m_SRVRangeDescs.count(name) == 0)
		return D3D12_ROOT_PARAMETER1();

	D3D12_ROOT_PARAMETER1 parameter = {};
	parameter.ShaderVisibility = visibility;
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameter.DescriptorTable.NumDescriptorRanges = 1;
	parameter.DescriptorTable.pDescriptorRanges = &m_ShaderRefResult.m_SRVRangeDescs.at(name);
	//parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(m_ShaderRefResult.m_SRVRangeDescs.size());
	//parameter.DescriptorTable.pDescriptorRanges = m_ShaderRefResult.m_SRVRangeDescs.data();

	return parameter;
}

D3D12_ROOT_PARAMETER1 Shader::GetCbvRootParameter(const D3D12_SHADER_VISIBILITY visibility, std::string name)const
{
	if (m_ShaderRefResult.m_CBVRangeDescs.count(name) == 0)
		return D3D12_ROOT_PARAMETER1();

	D3D12_ROOT_PARAMETER1 parameter = {};
	parameter.ShaderVisibility = visibility;
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameter.DescriptorTable.NumDescriptorRanges = 1;
	parameter.DescriptorTable.pDescriptorRanges = &m_ShaderRefResult.m_CBVRangeDescs.at(name).range;
	/*parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(m_ShaderRefResult.m_CBVRangeDescs.size());
	parameter.DescriptorTable.pDescriptorRanges = m_ShaderRefResult.m_CBVRangeDescs.data();*/
	return parameter;
}


DXGI_FORMAT VertexShader::GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
	if ((mask & 0x0F) == 0x0F)
	{
		//xyzw
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case D3D10_REGISTER_COMPONENT_SINT32:
			return DXGI_FORMAT_R32G32B32A32_SINT;
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			break;
		}
	}

	if ((mask & 0x07) == 0x07)
	{
		// xyz
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32_UINT;
		case D3D10_REGISTER_COMPONENT_SINT32:
			return DXGI_FORMAT_R32G32B32_SINT;
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		default:
			break;
		}
	}

	if ((mask & 0x3) == 0x3)
	{
		// xy
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32_UINT;
		case D3D10_REGISTER_COMPONENT_SINT32:
			return DXGI_FORMAT_R32G32_SINT;
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32_FLOAT;
		default:
			break;
		}
	}

	if ((mask & 0x1) == 0x1)
	{
		// x
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32_UINT;
		case D3D10_REGISTER_COMPONENT_SINT32:
			return DXGI_FORMAT_R32_SINT;
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32_FLOAT;
		default:
			break;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}

//頂点シェーダー
void VertexShader::Create(std::string fileName)
{
	m_FileName = fileName;
	LoadShaderFile();
	CalcShaderReflection(GetByteCode());
	CalcInputLayout();
}

void VertexShader::CalcInputLayout()
{
	D3D12_SHADER_DESC shaderDesc = {};
	m_ShaderRef->GetDesc(&shaderDesc);
	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D12_SIGNATURE_PARAMETER_DESC parameterDesc = {};
		m_ShaderRef->GetInputParameterDesc(i, &parameterDesc);

		//システムセマンティックス(SV_~~~)のやつは無視
		if (parameterDesc.SystemValueType != D3D_NAME_UNDEFINED)
			continue;

		auto format = GetDxgiFormat(parameterDesc.ComponentType, parameterDesc.Mask);

		D3D12_INPUT_ELEMENT_DESC element = {
			parameterDesc.SemanticName,
			parameterDesc.SemanticIndex,
			format,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};

		m_InputLayouts.push_back(element);
	}

}


//ピクセルシェーダー
void PixelShader::Create(std::string fileName)
{
	m_FileName = fileName;
	LoadShaderFile();
	CalcShaderReflection(GetByteCode());
}

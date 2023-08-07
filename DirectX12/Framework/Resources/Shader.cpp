#include "Shader.h"

D3D12_SHADER_BYTECODE Shader::GetByteCode()const
{
	D3D12_SHADER_BYTECODE byteCode = {};
	byteCode.BytecodeLength = m_Shader.Get()->GetBufferSize();
	byteCode.pShaderBytecode = m_Shader.Get()->GetBufferPointer();
	return byteCode;
}

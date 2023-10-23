#include "Material.h"
#include "Shader.h"
#include "GpuBuffer.h"
#include "../LowLevel/BufferView.h"
#include "../LowLevel/DescriptorHeap.h"
#include "../LowLevel/Dx12GraphicsDevice.h"

void Material::CalcConstantBuffer()
{
	ShaderReflectResult vRef = m_RenderSet->vShader->GetShaderReflectResult();
	ShaderReflectResult pRef = m_RenderSet->pShader->GetShaderReflectResult();
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	//TODO:::SRV�͏����ĂȂ�

	//vertex_shader
	for (auto desc : vRef.m_CBVRangeDescs)
	{
		std::string name = desc.first;
		CBVData data = desc.second;

		ConstantBuffer* cBuffer = new ConstantBuffer();
		cBuffer->CreateConstantBuffer(device, data.desc.Size);

		BufferView* cBufferView = new BufferView();
		//TODO:::�_�u���o�b�t�@�����O���l�����ق�����������
		DescriptorHeapManager::Instance().CreateConstantBufferView(cBuffer->GetResource().GetAddressOf(), cBufferView, 1);

		m_CBufferSet[name].reset(new CBufferSet(cBuffer, cBufferView));
	}

	//pixel_shader
	for (auto desc : pRef.m_CBVRangeDescs)
	{
		std::string name = desc.first;
		CBVData data = desc.second;

		ConstantBuffer* cBuffer = new ConstantBuffer();
		cBuffer->CreateConstantBuffer(device, data.desc.Size);

		BufferView* cBufferView = new BufferView();
		//TODO:::�_�u���o�b�t�@�����O���l�����ق�����������
		DescriptorHeapManager::Instance().CreateConstantBufferView(cBuffer->GetResource().GetAddressOf(), cBufferView, 1);


		m_CBufferSet[name].reset(new CBufferSet(cBuffer, cBufferView));
	}
}

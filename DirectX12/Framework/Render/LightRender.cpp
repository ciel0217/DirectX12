#include "LightRender.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../Resources/CLight.h"
#include "../LowLevel/DescriptorHeap.h"

void LightRender::SetUpRender()
{
	m_LightStructuredBuffer.reset(new StructuredBuffer());
	m_LightView.reset(new BufferView());

	auto device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	m_LightStructuredBuffer->CreateStructuredBuffer(device, MAX_LIGHT, sizeof(CLight));

	std::vector<D3D12_BUFFER_SRV> srvParam;
	srvParam.resize(1);
	srvParam[0].NumElements = MAX_LIGHT;
	srvParam[0].StructureByteStride = sizeof(CLight);

	DescriptorHeapManager::Instance().CreateStructuredBufferView(m_LightStructuredBuffer->GetResource().GetAddressOf(),m_LightView.get(), 1, srvParam);
}

void LightRender::UninitRender()
{
}

void LightRender::Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources, std::vector<std::shared_ptr<CLight>> lights, CommandListSet & commandListSet)
{
}

#include "MeshRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/DescriptorHeap.h"

void MeshRender::SetUpRender()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	m_WVPCBuffer.reset(new ConstantBuffer());
	m_WVPCBuffer->CreateConstantBuffer(device, sizeof(WVP));
	
	m_WVPCView.reset(new BufferView());
	DescriptorHeapManager::Intance().CreateConstantBufferView(m_WVPCBuffer->GetResource().GetAddressOf(), m_WVPCView.get(), 1);

}

void MeshRender::Draw()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	
	RootSignature* currentRootSignature = m_Material->GetRenderSet()->rootSignature;
	PipelineStateObject* pso = m_Material->GetRenderSet()->pipelineStateObj;


}
